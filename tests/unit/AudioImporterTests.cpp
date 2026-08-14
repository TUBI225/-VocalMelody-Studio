#include <vocalmelody/audio/AudioFileImporter.h>

#include "TestContext.h"

#include <juce_audio_formats/juce_audio_formats.h>

#include <cmath>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

namespace {
using vocalmelody::testing::TestContext;

void writeInt16(std::ofstream& file, const int16_t value) {
    const auto bytes = static_cast<uint16_t>(value);
    file.put(static_cast<char>(bytes & 0xFF));
    file.put(static_cast<char>((bytes >> 8) & 0xFF));
}

void writeInt32(std::ofstream& file, const int32_t value) {
    const auto bytes = static_cast<uint32_t>(value);
    file.put(static_cast<char>(bytes & 0xFF));
    file.put(static_cast<char>((bytes >> 8) & 0xFF));
    file.put(static_cast<char>((bytes >> 16) & 0xFF));
    file.put(static_cast<char>((bytes >> 24) & 0xFF));
}

void writePcm16Wav(const std::string& path, const int sampleRate, const int channels,
                   const std::vector<std::vector<float>>& samples) {
    const int numFrames = static_cast<int>(samples[0].size());
    constexpr int bytesPerSample = 2;
    const int blockAlign = channels * bytesPerSample;
    const int dataSize = numFrames * blockAlign;
    const int byteRate = sampleRate * blockAlign;

    std::ofstream file(path, std::ios::binary);
    file.write("RIFF", 4);
    writeInt32(file, 36 + dataSize);
    file.write("WAVE", 4);
    file.write("fmt ", 4);
    writeInt32(file, 16);
    writeInt16(file, 1);
    writeInt16(file, static_cast<int16_t>(channels));
    writeInt32(file, sampleRate);
    writeInt32(file, byteRate);
    writeInt16(file, static_cast<int16_t>(blockAlign));
    writeInt16(file, 16);
    file.write("data", 4);
    writeInt32(file, dataSize);
    for (int frame = 0; frame < numFrames; ++frame) {
        for (int ch = 0; ch < channels; ++ch) {
            const float value =
                samples[static_cast<std::size_t>(ch)][static_cast<std::size_t>(frame)];
            const auto sample = static_cast<int16_t>(value * 32767.0F);
            writeInt16(file, sample);
        }
    }
}

[[nodiscard]] std::string tempWavPath(const std::string& name) {
    const auto dir = juce::File::getSpecialLocation(juce::File::tempDirectory);
    return dir.getChildFile(name).getFullPathName().toStdString();
}

void testImportMonoWav(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const int sampleRate = 44100;
    const std::vector<std::vector<float>> samples(1, std::vector<float>(sampleRate, 0.25F));
    const std::string path = tempWavPath("vms_test_mono.wav");
    writePcm16Wav(path, sampleRate, 1, samples);

    const auto result = AudioFileImporter{}.import(path);
    context.expect(result.has_value(), "a valid mono wav imports successfully");
    if (!result.has_value()) {
        return;
    }

    context.expect(result->source.sampleRate() == sampleRate, "sample rate is preserved");
    context.expect(result->source.channelCount() == 1, "channel count is one");
    context.expect(std::abs(result->source.durationSeconds().value() - 1.0) < 0.01,
                   "a one second duration is detected");
    context.expect(result->analysis.analysisSampleRate() == sampleRate,
                   "analysis sample rate is preserved");
    context.expect(result->analysis.clippingScore().value() < 1e-6, "a 0.25 signal is not clipped");
    context.expect(std::abs(result->analysis.voicePresenceScore().value() - 1.0) < 1e-6,
                   "a constant signal is fully voiced");
}

void testImportStereoWav(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const int sampleRate = 22050;
    const std::vector<std::vector<float>> samples(2, std::vector<float>(sampleRate, 0.2F));
    const std::string path = tempWavPath("vms_test_stereo.wav");
    writePcm16Wav(path, sampleRate, 2, samples);

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

    const std::string path = tempWavPath("vms_test_invalid.bin");
    std::ofstream file(path, std::ios::binary);
    file.write("this is not an audio file at all", 31);

    context.expect(!AudioFileImporter{}.import(path).has_value(), "a non-audio file is rejected");
}

void testImportMissingFile(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    context.expect(!AudioFileImporter{}.import(tempWavPath("vms_test_missing.wav")).has_value(),
                   "a missing file is rejected");
}

} // namespace

int main() {
    TestContext context;
    testImportMonoWav(context);
    testImportStereoWav(context);
    testImportInvalidFile(context);
    testImportMissingFile(context);
    return context.result();
}
