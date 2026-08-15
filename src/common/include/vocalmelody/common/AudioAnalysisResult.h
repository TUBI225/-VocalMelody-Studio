#pragma once

#include <vocalmelody/common/StrongTypes.h>

#include <optional>
#include <string>
#include <vector>

namespace vocalmelody::common {

class SilenceSegment final {
  public:
    [[nodiscard]] static std::optional<SilenceSegment> create(const Seconds& start,
                                                              const Seconds& end) noexcept;

    [[nodiscard]] const Seconds& start() const noexcept { return start_; }
    [[nodiscard]] const Seconds& end() const noexcept { return end_; }

  private:
    SilenceSegment(Seconds start, Seconds end) noexcept;

    Seconds start_;
    Seconds end_;
};

class AudioAnalysisResult final {
  public:
    [[nodiscard]] static std::optional<AudioAnalysisResult>
    create(const std::string& audioSourceId, const int analysisVersion,
           const Seconds& durationSeconds, const int analysisSampleRate,
           const std::string& monoAnalysisPath, const Score01& clippingScore,
           const Score01& noiseScore, const Score01& voicePresenceScore,
           std::vector<SilenceSegment> silenceMap, const Score01& qualityScore,
           std::vector<std::string> warnings);

    [[nodiscard]] const std::string& audioSourceId() const noexcept { return audioSourceId_; }
    [[nodiscard]] int analysisVersion() const noexcept { return analysisVersion_; }
    [[nodiscard]] const Seconds& durationSeconds() const noexcept { return durationSeconds_; }
    [[nodiscard]] int analysisSampleRate() const noexcept { return analysisSampleRate_; }
    [[nodiscard]] const std::string& monoAnalysisPath() const noexcept { return monoAnalysisPath_; }
    [[nodiscard]] const Score01& clippingScore() const noexcept { return clippingScore_; }
    [[nodiscard]] const Score01& noiseScore() const noexcept { return noiseScore_; }
    [[nodiscard]] const Score01& voicePresenceScore() const noexcept { return voicePresenceScore_; }
    [[nodiscard]] const std::vector<SilenceSegment>& silenceMap() const noexcept {
        return silenceMap_;
    }
    [[nodiscard]] const Score01& qualityScore() const noexcept { return qualityScore_; }
    [[nodiscard]] const std::vector<std::string>& warnings() const noexcept { return warnings_; }

  private:
    AudioAnalysisResult(std::string audioSourceId, int analysisVersion, Seconds durationSeconds,
                        int analysisSampleRate, std::string monoAnalysisPath, Score01 clippingScore,
                        Score01 noiseScore, Score01 voicePresenceScore,
                        std::vector<SilenceSegment> silenceMap, Score01 qualityScore,
                        std::vector<std::string> warnings) noexcept;

    std::string audioSourceId_;
    int analysisVersion_;
    Seconds durationSeconds_;
    int analysisSampleRate_;
    std::string monoAnalysisPath_;
    Score01 clippingScore_;
    Score01 noiseScore_;
    Score01 voicePresenceScore_;
    std::vector<SilenceSegment> silenceMap_;
    Score01 qualityScore_;
    std::vector<std::string> warnings_;
};

} // namespace vocalmelody::common
