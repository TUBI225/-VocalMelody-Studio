#include <vocalmelody/audio/AudioFileImporter.h>
#include <vocalmelody/audio/AudioMetadataSerializer.h>

#include "TestContext.h"
#include "WavTestHelpers.h"

#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {
using vocalmelody::testing::TestContext;

void testMetadataSerialization(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;
    using vocalmelody::audio::audioMetadataToJson;

    const std::string wavPath = vocalmelody::testing::tempFilePath("vms_meta_test.wav");
    const std::vector<std::vector<float>> samples(1, std::vector<float>(44100, 0.25F));
    vocalmelody::testing::writePcm16Wav(wavPath, 44100, 1, samples);

    const auto result = AudioFileImporter{}.import(wavPath);
    context.expect(result.has_value(), "corpus wav imports");
    if (!result.has_value()) {
        return;
    }

    const std::string json = audioMetadataToJson(*result);
    context.expect(json.find("\"source\"") != std::string::npos, "json contains source");
    context.expect(json.find("\"analysis\"") != std::string::npos, "json contains analysis");
    context.expect(json.find("\"sampleRate\":44100") != std::string::npos,
                   "json contains the sample rate");
    context.expect(json.find("\"channelCount\":1") != std::string::npos,
                   "json contains the channel count");
    context.expect(json.find("\"clippingScore\"") != std::string::npos,
                   "json contains the clipping score");
    context.expect(json.find("\"silenceSegments\"") != std::string::npos,
                   "json contains the silence segments");
}

void testSaveMetadata(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;
    using vocalmelody::audio::saveAudioMetadata;

    const std::string wavPath = vocalmelody::testing::tempFilePath("vms_meta_save.wav");
    const std::vector<std::vector<float>> samples(1, std::vector<float>(22050, 0.1F));
    vocalmelody::testing::writePcm16Wav(wavPath, 22050, 1, samples);

    const auto result = AudioFileImporter{}.import(wavPath);
    context.expect(result.has_value(), "import works before saving metadata");
    if (!result.has_value()) {
        return;
    }

    const std::string metadataPath = vocalmelody::testing::tempFilePath("vms_meta.json");
    context.expect(saveAudioMetadata(*result, metadataPath), "the metadata file is written");

    std::ifstream file(metadataPath);
    const std::string content((std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>());
    context.expect(content.find("\"originalFormat\":\"wav\"") != std::string::npos,
                   "saved metadata contains the format");
    context.expect(content.find("\"analysisVersion\":2") != std::string::npos,
                   "saved metadata contains the resampled analysis version");
}

void testSourceFileUnchanged(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const std::string wavPath = vocalmelody::testing::tempFilePath("vms_meta_unchanged.wav");
    const std::vector<std::vector<float>> samples(2, std::vector<float>(8000, 0.3F));
    vocalmelody::testing::writePcm16Wav(wavPath, 8000, 2, samples);

    const std::string before = vocalmelody::testing::fnv1aFileHash(wavPath);
    const auto result = AudioFileImporter{}.import(wavPath);
    const std::string after = vocalmelody::testing::fnv1aFileHash(wavPath);

    context.expect(result.has_value(), "import works on the corpus file");
    context.expect(!before.empty() && before == after, "the original audio file is never modified");
}

void testCorpusDiagnostics(TestContext& context) {
    using vocalmelody::audio::AudioFileImporter;

    const std::string clippedPath = vocalmelody::testing::tempFilePath("vms_meta_clipped.wav");
    std::vector<std::vector<float>> clipped(1, std::vector<float>(44100, 0.0F));
    for (std::size_t i = 0; i < clipped[0].size(); i += 100) {
        clipped[0][i] = 1.2F;
    }
    vocalmelody::testing::writePcm16Wav(clippedPath, 44100, 1, clipped);
    const auto clippedResult = AudioFileImporter{}.import(clippedPath);
    context.expect(clippedResult.has_value() &&
                       clippedResult->analysis.clippingScore().value() > 0.0,
                   "clipping is detected on the clipped corpus file");

    const std::string silentPath = vocalmelody::testing::tempFilePath("vms_meta_silent.wav");
    const std::vector<std::vector<float>> silent(1, std::vector<float>(44100, 0.0F));
    vocalmelody::testing::writePcm16Wav(silentPath, 44100, 1, silent);
    const auto silentResult = AudioFileImporter{}.import(silentPath);
    context.expect(silentResult.has_value() &&
                       silentResult->analysis.voicePresenceScore().value() < 0.01,
                   "silence is detected on the silent corpus file");
}

} // namespace

int main() {
    TestContext context;
    testMetadataSerialization(context);
    testSaveMetadata(context);
    testSourceFileUnchanged(context);
    testCorpusDiagnostics(context);
    return context.result();
}
