#include <vocalmelody/audio/Mp3Decoder.h>

#include "Mp3TestHelpers.h"
#include "TestContext.h"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace {
using vocalmelody::audio::Mp3Decoder;
using vocalmelody::testing::TestContext;

void testDecodePinnedLayer3Vector(TestContext& context) {
    const std::string path = vocalmelody::testing::tempFilePath("vms_test_layer3.mp3");
    vocalmelody::testing::copyMp3TestVector(path);

    const auto result = Mp3Decoder::decodeFile(path);
    context.expect(result.has_value(), "the pinned Layer III test vector decodes successfully");
    if (result.has_value()) {
        context.expect(result->sampleRate > 0, "the test vector sample rate is detected");
        context.expect(result->channelCount >= 1 && result->channelCount <= 2,
                       "the test vector channel count is valid");
        context.expect(result->totalFrames > 0, "frames are decoded");
        context.expect(result->monoSamples.size() == static_cast<std::size_t>(result->totalFrames),
                       "mono samples size matches total frames");
        context.expect(std::any_of(result->monoSamples.begin(), result->monoSamples.end(),
                                   [](const float sample) { return std::abs(sample) > 0.01F; }),
                       "the Layer III vector decodes to a non-silent signal");
    }
}

void testDecodeMemory(TestContext& context) {
    const auto data = vocalmelody::testing::readMp3TestVector();
    const auto result = Mp3Decoder::decodeMemory(data);
    context.expect(result.has_value(), "decoding from memory buffer succeeds");
    if (result.has_value()) {
        context.expect(result->channelCount >= 1 && result->channelCount <= 2,
                       "memory decoded channel count is valid");
        context.expect(result->sampleRate > 0, "memory decoded sample rate is valid");
    }

    const auto emptyResult = Mp3Decoder::decodeMemory({});
    context.expect(!emptyResult.has_value(), "empty memory buffer is rejected");

    const std::vector<std::uint8_t> corrupted{'I', 'D', '3', 3,   0,   0,   0,   0,
                                              0,   8,   'n', 'o', 't', 'm', 'p', '3'};
    context.expect(!Mp3Decoder::decodeMemory(corrupted).has_value(),
                   "corrupted memory buffer is rejected");
}

void testDecodeEmptyAndCorruptedFile(TestContext& context) {
    const std::string emptyPath = vocalmelody::testing::tempFilePath("vms_test_empty.mp3");
    {
        std::ofstream file(emptyPath, std::ios::binary);
    }
    context.expect(!Mp3Decoder::decodeFile(emptyPath).has_value(), "an empty MP3 is rejected");

    const std::string corruptedPath = vocalmelody::testing::tempFilePath("vms_test_corrupt.mp3");
    {
        std::ofstream file(corruptedPath, std::ios::binary);
        file.write("ID3\x03\0\0\0\0\0\x10this is corrupted mp3 content", 40);
    }
    context.expect(!Mp3Decoder::decodeFile(corruptedPath).has_value(),
                   "a corrupted MP3 is rejected without crash");
}

void testDecodeNonExistentFile(TestContext& context) {
    const std::string missingPath =
        vocalmelody::testing::tempFilePath("vms_test_non_existent_file.mp3");
    context.expect(!Mp3Decoder::decodeFile(missingPath).has_value(),
                   "a non-existent file is rejected safely");
}

void testDecodeTruncatedCbrDocumented(TestContext& context) {
    // Invariant documenté (Mp3Decoder.h) : un flux CBR tronqué en fin de
    // fichier se décode sans erreur, le dernier frame incomplet étant ignoré
    // (comportement standard des décodeurs ; minimp3 ne signale pas cette
    // condition). Ce test fige le comportement : décodage réussi avec un
    // nombre de frames réduit, sans crash.
    const std::string fullPath = vocalmelody::testing::tempFilePath("vms_test_layer3_full.mp3");
    vocalmelody::testing::copyMp3TestVector(fullPath);
    const auto full = Mp3Decoder::decodeFile(fullPath);
    context.expect(full.has_value(), "the full pinned vector decodes");

    const std::string path = vocalmelody::testing::tempFilePath("vms_test_layer3_truncated.mp3");
    const auto data = vocalmelody::testing::readMp3TestVector();
    const auto truncatedSize = data.size() > 1024 ? data.size() - 512 : data.size() / 2;
    const std::vector<std::uint8_t> truncated(data.begin(), data.begin() + truncatedSize);
    {
        std::ofstream file(std::filesystem::path(std::u8string(path.begin(), path.end())),
                           std::ios::binary);
        file.write(reinterpret_cast<const char*>(truncated.data()),
                   static_cast<std::streamsize>(truncated.size()));
    }

    const auto result = Mp3Decoder::decodeFile(path);
    context.expect(result.has_value(),
                   "a truncated CBR mp3 decodes without error (documented invariant)");
    if (result.has_value() && full.has_value()) {
        context.expect(result->totalFrames < full->totalFrames,
                       "the truncated file reports fewer decoded frames");
    }
}

} // namespace

int main() {
    TestContext context;
    testDecodePinnedLayer3Vector(context);
    testDecodeMemory(context);
    testDecodeEmptyAndCorruptedFile(context);
    testDecodeNonExistentFile(context);
    testDecodeTruncatedCbrDocumented(context);
    return context.result();
}
