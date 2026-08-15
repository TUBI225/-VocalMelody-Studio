#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace vocalmelody::audio {

struct DecodedAudioData final {
    int sampleRate{0};
    int channelCount{0};
    int bitDepth{16};
    std::vector<float> monoSamples;
    std::int64_t totalFrames{0};
};

class Mp3Decoder final {
  public:
    static constexpr std::size_t kMaxMp3SizeBytes = 1024ULL * 1024ULL * 1024ULL; // 1 Gio
    static constexpr std::size_t kMaxDecodedFrames = 30'000'000ULL;

    Mp3Decoder() = delete;

    [[nodiscard]] static std::optional<DecodedAudioData>
    decodeFile(const std::string& filePath) noexcept;

    [[nodiscard]] static std::optional<DecodedAudioData>
    decodeMemory(std::span<const std::uint8_t> data) noexcept;
};

} // namespace vocalmelody::audio
