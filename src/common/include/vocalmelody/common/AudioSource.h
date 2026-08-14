#pragma once

#include <vocalmelody/common/StrongTypes.h>

#include <optional>
#include <string>
#include <string_view>

namespace vocalmelody::common {

enum class AudioFormat {
    Unknown,
    Wav,
    Mp3,
    M4a,
};

[[nodiscard]] AudioFormat audioFormatFromExtension(const std::string_view extension) noexcept;
[[nodiscard]] std::string_view audioFormatToString(const AudioFormat format) noexcept;

class AudioSource final {
  public:
    [[nodiscard]] static std::optional<AudioSource>
    create(const std::string& id, const std::string& originalPath, const std::string& importedAt,
           const AudioFormat originalFormat, const int sampleRate, const int channelCount,
           const int bitDepth, const Seconds& durationSeconds,
           const std::string& fileHash) noexcept;

    [[nodiscard]] const std::string& id() const noexcept { return id_; }
    [[nodiscard]] const std::string& originalPath() const noexcept { return originalPath_; }
    [[nodiscard]] const std::string& importedAt() const noexcept { return importedAt_; }
    [[nodiscard]] AudioFormat originalFormat() const noexcept { return originalFormat_; }
    [[nodiscard]] int sampleRate() const noexcept { return sampleRate_; }
    [[nodiscard]] int channelCount() const noexcept { return channelCount_; }
    [[nodiscard]] int bitDepth() const noexcept { return bitDepth_; }
    [[nodiscard]] const Seconds& durationSeconds() const noexcept { return durationSeconds_; }
    [[nodiscard]] const std::string& fileHash() const noexcept { return fileHash_; }

  private:
    AudioSource(std::string id, std::string originalPath, std::string importedAt,
                AudioFormat originalFormat, int sampleRate, int channelCount, int bitDepth,
                Seconds durationSeconds, std::string fileHash) noexcept;

    std::string id_;
    std::string originalPath_;
    std::string importedAt_;
    AudioFormat originalFormat_;
    int sampleRate_;
    int channelCount_;
    int bitDepth_;
    Seconds durationSeconds_;
    std::string fileHash_;
};

} // namespace vocalmelody::common
