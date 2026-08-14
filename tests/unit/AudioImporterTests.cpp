#include <vocalmelody/audio/AudioFileImporter.h>

#include "TestContext.h"
#include "WavTestHelpers.h"

#include <cmath>
#include <fstream>
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

void testImportMissingFile(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    context.expect(!AudioFileImporter{}
                        .import(vocalmelody::testing::tempFilePath("vms_test_missing.wav"))
                        .has_value(),
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
