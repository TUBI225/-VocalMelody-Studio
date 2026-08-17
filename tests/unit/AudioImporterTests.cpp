#include <vocalmelody/audio/AudioFileImporter.h>

#include "Mp3TestHelpers.h"
#include "TestContext.h"
#include "WavTestHelpers.h"

#include <cmath>
#include <fstream>
#include <limits>
#include <string>
#include <vector>

namespace {
using vocalmelody::testing::TestContext;

void testImportMonoWav(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const int sampleRate = 44100;
    const std::vector<std::vector<float>> samples(1, std::vector<float>(sampleRate, 0.25F));
    const std::string path = vocalmelody::testing::tempFilePath("vms_test_mono.wav");
    vocalmelody::testing::writePcm16Wav(path, sampleRate, 1, samples);

    const auto result = AudioFileImporter{}.import(path);
    context.expect(result.has_value(), "a valid mono wav imports successfully");
    if (!result.has_value()) {
        return;
    }

    context.expect(result->source.sampleRate() == sampleRate, "sample rate is preserved");
    context.expect(result->source.channelCount() == 1, "channel count is one");
    context.expect(std::abs(result->source.durationSeconds().value() - 1.0) < 0.01,
                   "a one second duration is detected");
    context.expect(result->analysis.analysisSampleRate() == AudioFileImporter::kAnalysisSampleRate,
                   "analysis is resampled to the canonical sample rate");
    context.expect(result->analysis.analysisVersion() == AudioFileImporter::kAnalysisVersion,
                   "analysis metadata identifies the filtered resampler version");
    context.expect(result->analysis.clippingScore().value() < 1e-6, "a 0.25 signal is not clipped");
    context.expect(std::abs(result->analysis.voicePresenceScore().value() - 1.0) < 1e-6,
                   "a constant signal is fully voiced");
    context.expect(result->source.fileHash().size() == 64, "the source uses a SHA-256 file hash");
    context.expect(result->source.id() == "audio-" + result->source.fileHash(),
                   "the source id is stable and content-derived");
    context.expect(result->source.importedAt().find('T') != std::string::npos,
                   "the import timestamp is generated at runtime");
}

void testImportMultichannelWav(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const int sampleRate = 8000;
    std::vector<std::vector<float>> samples(3, std::vector<float>(sampleRate, 0.0F));
    samples[0].assign(static_cast<std::size_t>(sampleRate), 0.6F);
    samples[1].assign(static_cast<std::size_t>(sampleRate), -0.3F);
    samples[2].assign(static_cast<std::size_t>(sampleRate), 0.3F);
    const std::string path = vocalmelody::testing::tempFilePath("vms_test_multichannel.wav");
    vocalmelody::testing::writePcm16Wav(path, sampleRate, 3, samples);

    const auto result = AudioFileImporter{}.import(path);
    context.expect(result.has_value(), "a valid three-channel wav imports successfully");
    if (result.has_value()) {
        context.expect(result->source.channelCount() == 3, "all source channels are reported");
        context.expect(result->analysis.voicePresenceScore().value() > 0.99,
                       "all channels contribute to the mono analysis");
    }
}

void testImportEmptyShortAndLongWav(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const std::string emptyPath = vocalmelody::testing::tempFilePath("vms_test_empty.wav");
    vocalmelody::testing::writePcm16Wav(emptyPath, 44100, 1, {{}});
    context.expect(!AudioFileImporter{}.import(emptyPath).has_value(), "an empty wav is rejected");

    const std::string shortPath = vocalmelody::testing::tempFilePath("vms_test_short.wav");
    vocalmelody::testing::writePcm16Wav(shortPath, 44100, 1, {{0.25F}});
    context.expect(AudioFileImporter{}.import(shortPath).has_value(),
                   "a one-frame wav imports without crashing");

    constexpr int longSampleRate = 8000;
    constexpr int longDurationSeconds = 30;
    const std::string longPath = vocalmelody::testing::tempFilePath("vms_test_long.wav");
    const std::vector<std::vector<float>> longSamples(
        1, std::vector<float>(longSampleRate * longDurationSeconds, 0.1F));
    vocalmelody::testing::writePcm16Wav(longPath, longSampleRate, 1, longSamples);
    const auto longResult = AudioFileImporter{}.import(longPath);
    context.expect(longResult.has_value(), "a thirty-second wav imports without crashing");
    if (longResult.has_value()) {
        context.expect(std::abs(longResult->source.durationSeconds().value() - 30.0) < 0.01,
                       "the long-file duration is detected");
    }
}

void testImportCorruptedWav(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const std::string path = vocalmelody::testing::tempFilePath("vms_test_corrupted.wav");
    std::ofstream file(path, std::ios::binary);
    file.write("RIFF\x24\0\0\0WAVEfmt ", 16);
    file.close();

    context.expect(!AudioFileImporter{}.import(path).has_value(),
                   "a truncated wav is rejected without crashing");
}

void testImportStereoWav(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const int sampleRate = 22050;
    const std::vector<std::vector<float>> samples(2, std::vector<float>(sampleRate, 0.2F));
    const std::string path = vocalmelody::testing::tempFilePath("vms_test_stereo.wav");
    vocalmelody::testing::writePcm16Wav(path, sampleRate, 2, samples);

    const auto result = AudioFileImporter{}.import(path);
    context.expect(result.has_value(), "a valid stereo wav imports successfully");
    if (!result.has_value()) {
        return;
    }

    context.expect(result->source.channelCount() == 2, "channel count is two");
    context.expect(result->source.sampleRate() == sampleRate, "stereo sample rate is preserved");
    context.expect(result->analysis.voicePresenceScore().value() > 0.99,
                   "the downmixed stereo signal is voiced");
}

void testImportInvalidFile(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const std::string path = vocalmelody::testing::tempFilePath("vms_test_invalid.bin");
    std::ofstream file(path, std::ios::binary);
    file.write("this is not an audio file at all", 31);

    context.expect(!AudioFileImporter{}.import(path).has_value(), "a non-audio file is rejected");
}

void testImportValidMp3(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const std::string path = vocalmelody::testing::tempFilePath("vms_test_import.mp3");
    vocalmelody::testing::copyMp3TestVector(path);

    const auto result = AudioFileImporter{}.import(path);
    context.expect(result.has_value(), "a valid mp3 imports successfully");
    if (!result.has_value()) {
        return;
    }

    context.expect(result->source.sampleRate() > 0, "mp3 sample rate is detected");
    context.expect(result->source.channelCount() >= 1 && result->source.channelCount() <= 2,
                   "mp3 channel count is valid");
    context.expect(result->source.originalFormat() == vocalmelody::common::AudioFormat::Mp3,
                   "mp3 format is detected");
    context.expect(result->analysis.analysisSampleRate() == AudioFileImporter::kAnalysisSampleRate,
                   "mp3 is resampled to canonical 16 kHz for analysis");
    context.expect(result->source.fileHash().size() == 64, "the source has a valid SHA-256 hash");
}

void testImportCorruptedMp3(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const std::string path = vocalmelody::testing::tempFilePath("vms_test_corrupted.mp3");
    std::ofstream file(path, std::ios::binary);
    file.write("ID3\x03\0\0\0\0\0\x20truncated_mp3_garbage_content_12345", 44);
    file.close();

    context.expect(!AudioFileImporter{}.import(path).has_value(),
                   "a corrupted mp3 is rejected safely");
}

void testImportMp3WithWrongExtension(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const std::string path = vocalmelody::testing::tempFilePath("vms_test_mp3_renamed.wav");
    vocalmelody::testing::copyMp3TestVector(path);
    context.expect(!AudioFileImporter{}.import(path).has_value(),
                   "an MP3 renamed as WAV is rejected instead of mislabelled");
}

void testImportMissingFile(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    context.expect(!AudioFileImporter{}
                        .import(vocalmelody::testing::tempFilePath("vms_test_missing.wav"))
                        .has_value(),
                   "a missing file is rejected");
}

void testImportForgedHeaders(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    // En-tête déclarant plus de canaux que le plafond (65 > 64) : rejeté avant
    // toute allocation.
    const std::string manyChannels = vocalmelody::testing::tempFilePath("vms_test_forged_65ch.wav");
    vocalmelody::testing::writeForgedWavHeader(manyChannels, 65, 1);
    context.expect(!AudioFileImporter{}.import(manyChannels).has_value(),
                   "a wav declaring 65 channels is rejected before allocation");

    // Borne maximale du champ canaux (65 535) : rejeté sans allocation massive.
    const std::string maxChannels =
        vocalmelody::testing::tempFilePath("vms_test_forged_65535ch.wav");
    vocalmelody::testing::writeForgedWavHeader(maxChannels, 65535, 1);
    context.expect(!AudioFileImporter{}.import(maxChannels).has_value(),
                   "a wav declaring 65535 channels is rejected before allocation");

    // 64 canaux x 2 M trames déclarées : le produit (128 M) dépasse le plafond
    // total d'échantillons décodés (100 M).
    const std::string manySamples =
        vocalmelody::testing::tempFilePath("vms_test_forged_total_samples.wav");
    vocalmelody::testing::writeForgedWavHeader(manySamples, 64, 2'000'000);
    context.expect(!AudioFileImporter{}.import(manySamples).has_value(),
                   "a wav exceeding the total decoded sample cap is rejected");
}

} // namespace

int main() {
    TestContext context;
    testImportMonoWav(context);
    testImportStereoWav(context);
    testImportMultichannelWav(context);
    testImportEmptyShortAndLongWav(context);
    testImportCorruptedWav(context);
    testImportValidMp3(context);
    testImportCorruptedMp3(context);
    testImportMp3WithWrongExtension(context);
    testImportInvalidFile(context);
    testImportMissingFile(context);
    testImportForgedHeaders(context);
    return context.result();
}
