#pragma once

#include <vocalmelody/common/AudioAnalysisResult.h>
#include <vocalmelody/common/AudioSource.h>

#include <optional>
#include <string>

namespace vocalmelody::audio {

struct AudioImportResult final {
    common::AudioSource source;
    common::AudioAnalysisResult analysis;
};

class AudioFileImporter final {
  public:
    AudioFileImporter();

    [[nodiscard]] std::optional<AudioImportResult> import(const std::string& path) const noexcept;

  private:
    [[nodiscard]] static std::string computeFileHash(const std::string& path) noexcept;
};

} // namespace vocalmelody::audio
