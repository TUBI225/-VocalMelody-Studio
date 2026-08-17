#pragma once

#include <vocalmelody/common/AudioAnalysisResult.h>
#include <vocalmelody/common/AudioSource.h>

#include <cstdint>
#include <optional>
#include <string>

namespace vocalmelody::audio {

struct AudioImportResult final {
    common::AudioSource source;
    common::AudioAnalysisResult analysis;
};

class AudioFileImporter final {
  public:
    static constexpr std::int64_t kMaxFileSizeBytes = 1024LL * 1024LL * 1024LL;
    static constexpr std::int64_t kMaxDecodedFrames = 30'000'000LL;
    // Borne le buffer décodé (canaux x trames) avant toute allocation : un
    // en-tête forgé peut déclarer jusqu'à 65 535 canaux (champ uint16 du WAV).
    static constexpr int kMaxChannelCount = 64;
    static constexpr std::int64_t kMaxDecodedSamples = 100'000'000LL;
    static constexpr int kAnalysisSampleRate = 16'000;
    static constexpr int kAnalysisVersion = 4;

    AudioFileImporter();

    [[nodiscard]] std::optional<AudioImportResult> import(const std::string& path) const noexcept;

  private:
    [[nodiscard]] static std::string computeFileHash(const std::string& path);
};

} // namespace vocalmelody::audio
