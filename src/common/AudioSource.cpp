#include <vocalmelody/common/AudioSource.h>

#include <algorithm>
#include <cctype>
#include <string>
#include <utility>

namespace vocalmelody::common {

AudioFormat audioFormatFromExtension(const std::string_view extension) noexcept {
    if (extension.empty()) {
        return AudioFormat::Unknown;
    }

    std::string_view normalized = extension;
    if (normalized.front() == '.') {
        normalized.remove_prefix(1);
    }
    if (normalized.empty()) {
        return AudioFormat::Unknown;
    }

    auto lower = std::string(normalized);
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](const unsigned char c) { return static_cast<char>(std::tolower(c)); });

    if (lower == "wav") {
        return AudioFormat::Wav;
    }
    if (lower == "mp3") {
        return AudioFormat::Mp3;
    }
    if (lower == "m4a") {
        return AudioFormat::M4a;
    }
    return AudioFormat::Unknown;
}

std::string_view audioFormatToString(const AudioFormat format) noexcept {
    switch (format) {
    case AudioFormat::Wav:
        return "wav";
    case AudioFormat::Mp3:
        return "mp3";
    case AudioFormat::M4a:
        return "m4a";
    case AudioFormat::Unknown:
        break;
    }
    return "unknown";
}

std::optional<AudioSource>
AudioSource::create(const std::string& id, const std::string& originalPath,
                    const std::string& importedAt, const AudioFormat originalFormat,
                    const int sampleRate, const int channelCount, const int bitDepth,
                    const Seconds& durationSeconds, const std::string& fileHash) noexcept {
    if (id.empty() || originalPath.empty() || importedAt.empty() || fileHash.empty()) {
        return std::nullopt;
    }
    if (sampleRate <= 0 || channelCount <= 0 || bitDepth <= 0) {
        return std::nullopt;
    }
    if (originalFormat == AudioFormat::Unknown) {
        return std::nullopt;
    }

    return AudioSource{id,           originalPath, importedAt,      originalFormat, sampleRate,
                       channelCount, bitDepth,     durationSeconds, fileHash};
}

AudioSource::AudioSource(std::string id, std::string originalPath, std::string importedAt,
                         const AudioFormat originalFormat, const int sampleRate,
                         const int channelCount, const int bitDepth, Seconds durationSeconds,
                         std::string fileHash) noexcept
    : id_{std::move(id)}, originalPath_{std::move(originalPath)},
      importedAt_{std::move(importedAt)}, originalFormat_{originalFormat}, sampleRate_{sampleRate},
      channelCount_{channelCount}, bitDepth_{bitDepth},
      durationSeconds_{std::move(durationSeconds)}, fileHash_{std::move(fileHash)} {}

} // namespace vocalmelody::common
