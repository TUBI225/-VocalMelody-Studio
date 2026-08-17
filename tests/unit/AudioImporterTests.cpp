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

    const auto outcome = AudioFileImporter{}.import(path);
    context.expect(outcome.has_value(), "a valid mono wav imports successfully");
    if (!outcome.has_value()) {
        return;
    }

    context.expect(outcome->source.sampleRate() == sampleRate, "sample rate is preserved");
    context.expect(outcome->source.channelCount() == 1, "channel count is one");
    context.expect(std::abs(outcome->source.durationSeconds().value() - 1.0) < 0.01,
                   "a one second duration is detected");
    context.expect(outcome->analysis.analysisSampleRate() == AudioFileImporter::kAnalysisSampleRate,
                   "analysis is resampled to the canonical sample rate");
    context.expect(outcome->analysis.analysisVersion() == AudioFileImporter::kAnalysisVersion,
                   "analysis metadata identifies the filtered resampler version");
    context.expect(outcome->analysis.clippingScore().value() < 1e-6,
                   "a 0.25 signal is not clipped");
    context.expect(std::abs(outcome->analysis.voicePresenceScore().value() - 1.0) < 1e-6,
                   "a constant signal is fully voiced");
    context.expect(outcome->source.fileHash().size() == 64, "the source uses a SHA-256 file hash");
    context.expect(outcome->source.id() == "audio-" + outcome->source.fileHash(),
                   "the source id is stable and content-derived");
    context.expect(outcome->source.importedAt().find('T') != std::string::npos,
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

    const auto outcome = AudioFileImporter{}.import(path);
    context.expect(outcome.has_value(), "a valid three-channel wav imports successfully");
    if (outcome.has_value()) {
        context.expect(outcome->source.channelCount() == 3, "all source channels are reported");
        context.expect(outcome->analysis.voicePresenceScore().value() > 0.99,
                       "all channels contribute to the mono analysis");
    }
}

void testImportEmptyShortAndLongWav(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const std::string emptyPath = vocalmelody::testing::tempFilePath("vms_test_empty.wav");
    vocalmelody::testing::writePcm16Wav(emptyPath, 44100, 1, {{}});
    const auto emptyOutcome = AudioFileImporter{}.import(emptyPath);
    context.expect(!emptyOutcome.has_value(), "an empty wav is rejected");
    context.expect(emptyOutcome.error() == vocalmelody::audio::ImportError::DecodeFailed,
                   "an empty wav reports a decode failure");

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
    const auto longOutcome = AudioFileImporter{}.import(longPath);
    context.expect(longOutcome.has_value(), "a thirty-second wav imports without crashing");
    if (longOutcome.has_value()) {
        context.expect(std::abs(longOutcome->source.durationSeconds().value() - 30.0) < 0.01,
                       "the long-file duration is detected");
    }
}

void testImportCorruptedWav(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const std::string path = vocalmelody::testing::tempFilePath("vms_test_corrupted.wav");
    std::ofstream file(path, std::ios::binary);
    file.write("RIFF\x24\0\0\0WAVEfmt ", 16);
    file.close();

    const auto outcome = AudioFileImporter{}.import(path);
    context.expect(!outcome.has_value(), "a truncated wav is rejected without crashing");
    context.expect(outcome.error() == vocalmelody::audio::ImportError::DecodeFailed,
                   "a truncated wav reports a decode failure");
}

void testImportStereoWav(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const int sampleRate = 22050;
    const std::vector<std::vector<float>> samples(2, std::vector<float>(sampleRate, 0.2F));
    const std::string path = vocalmelody::testing::tempFilePath("vms_test_stereo.wav");
    vocalmelody::testing::writePcm16Wav(path, sampleRate, 2, samples);

    const auto outcome = AudioFileImporter{}.import(path);
    context.expect(outcome.has_value(), "a valid stereo wav imports successfully");
    if (!outcome.has_value()) {
        return;
    }

    context.expect(outcome->source.channelCount() == 2, "channel count is two");
    context.expect(outcome->source.sampleRate() == sampleRate, "stereo sample rate is preserved");
    context.expect(outcome->analysis.voicePresenceScore().value() > 0.99,
                   "the downmixed stereo signal is voiced");
}

void testImportInvalidFile(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const std::string path = vocalmelody::testing::tempFilePath("vms_test_invalid.bin");
    std::ofstream file(path, std::ios::binary);
    file.write("this is not an audio file at all", 31);
    file.close();

    const auto outcome = AudioFileImporter{}.import(path);
    context.expect(!outcome.has_value(), "a non-audio file is rejected");
    context.expect(outcome.error() == vocalmelody::audio::ImportError::UnsupportedFormat,
                   "a non-audio file reports an unsupported format");
}

void testImportValidMp3(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const std::string path = vocalmelody::testing::tempFilePath("vms_test_import.mp3");
    vocalmelody::testing::copyMp3TestVector(path);

    const auto outcome = AudioFileImporter{}.import(path);
    context.expect(outcome.has_value(), "a valid mp3 imports successfully");
    if (!outcome.has_value()) {
        return;
    }

    context.expect(outcome->source.sampleRate() > 0, "mp3 sample rate is detected");
    context.expect(outcome->source.channelCount() >= 1 && outcome->source.channelCount() <= 2,
                   "mp3 channel count is valid");
    context.expect(outcome->source.originalFormat() == vocalmelody::common::AudioFormat::Mp3,
                   "mp3 format is detected");
    context.expect(outcome->analysis.analysisSampleRate() == AudioFileImporter::kAnalysisSampleRate,
                   "mp3 is resampled to canonical 16 kHz for analysis");
    context.expect(outcome->source.fileHash().size() == 64, "the source has a valid SHA-256 hash");
}

void testImportCorruptedMp3(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const std::string path = vocalmelody::testing::tempFilePath("vms_test_corrupted.mp3");
    std::ofstream file(path, std::ios::binary);
    file.write("ID3\x03\0\0\0\0\0\x20truncated_mp3_garbage_content_12345", 44);
    file.close();

    const auto outcome = AudioFileImporter{}.import(path);
    context.expect(!outcome.has_value(), "a corrupted mp3 is rejected safely");
    context.expect(outcome.error() == vocalmelody::audio::ImportError::DecodeFailed,
                   "a corrupted mp3 reports a decode failure");
}

void testImportMp3WithWrongExtension(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const std::string path = vocalmelody::testing::tempFilePath("vms_test_mp3_renamed.wav");
    vocalmelody::testing::copyMp3TestVector(path);
    const auto outcome = AudioFileImporter{}.import(path);
    context.expect(!outcome.has_value(),
                   "an MP3 renamed as WAV is rejected instead of mislabelled");
    context.expect(outcome.error() == vocalmelody::audio::ImportError::DecodeFailed,
                   "an MP3 renamed as WAV reports a decode failure");
}

void testImportMissingFile(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const auto outcome =
        AudioFileImporter{}.import(vocalmelody::testing::tempFilePath("vms_test_missing.wav"));
    context.expect(!outcome.has_value(), "a missing file is rejected");
    context.expect(outcome.error() == vocalmelody::audio::ImportError::FileNotFound,
                   "a missing file reports not found");
}

void testImportForgedHeaders(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    // En-tête déclarant plus de canaux que le plafond (65 > 64) : rejeté avant
    // toute allocation.
    const std::string manyChannels = vocalmelody::testing::tempFilePath("vms_test_forged_65ch.wav");
    vocalmelody::testing::writeForgedWavHeader(manyChannels, 65, 1);
    const auto manyOutcome = AudioFileImporter{}.import(manyChannels);
    context.expect(!manyOutcome.has_value(),
                   "a wav declaring 65 channels is rejected before allocation");
    context.expect(manyOutcome.error() == vocalmelody::audio::ImportError::DecodeFailed,
                   "a wav declaring 65 channels reports a decode failure");

    // Borne maximale du champ canaux (65 535) : rejeté sans allocation massive.
    const std::string maxChannels =
        vocalmelody::testing::tempFilePath("vms_test_forged_65535ch.wav");
    vocalmelody::testing::writeForgedWavHeader(maxChannels, 65535, 1);
    const auto maxOutcome = AudioFileImporter{}.import(maxChannels);
    context.expect(!maxOutcome.has_value(),
                   "a wav declaring 65535 channels is rejected before allocation");

    // 64 canaux x 2 M trames déclarées : le produit (128 M) dépasse le plafond
    // total d'échantillons décodés (100 M).
    const std::string manySamples =
        vocalmelody::testing::tempFilePath("vms_test_forged_total_samples.wav");
    vocalmelody::testing::writeForgedWavHeader(manySamples, 64, 2'000'000);
    const auto samplesOutcome = AudioFileImporter{}.import(manySamples);
    context.expect(!samplesOutcome.has_value(),
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
