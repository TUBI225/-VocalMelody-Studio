#include <vocalmelody/common/AudioAnalysisResult.h>

#include <utility>

namespace vocalmelody::common {

std::optional<SilenceSegment> SilenceSegment::create(const Seconds& start,
                                                     const Seconds& end) noexcept {
    if (!(start < end)) {
        return std::nullopt;
    }
    return SilenceSegment{start, end};
}

SilenceSegment::SilenceSegment(Seconds start, Seconds end) noexcept
    : start_{std::move(start)}, end_{std::move(end)} {}

std::optional<AudioAnalysisResult>
AudioAnalysisResult::create(const std::string& audioSourceId, const int analysisVersion,
                            const Seconds& durationSeconds, const int analysisSampleRate,
                            const std::string& monoAnalysisPath, const Score01& clippingScore,
                            const Score01& noiseScore, const Score01& voicePresenceScore,
                            std::vector<SilenceSegment> silenceMap, const Score01& qualityScore,
                            std::vector<std::string> warnings) noexcept {
    if (audioSourceId.empty() || analysisVersion < 0 || analysisSampleRate <= 0) {
        return std::nullopt;
    }

    return AudioAnalysisResult{audioSourceId,      analysisVersion,    durationSeconds,
                               analysisSampleRate, monoAnalysisPath,   clippingScore,
                               noiseScore,         voicePresenceScore, std::move(silenceMap),
                               qualityScore,       std::move(warnings)};
}

AudioAnalysisResult::AudioAnalysisResult(std::string audioSourceId, const int analysisVersion,
                                         Seconds durationSeconds, const int analysisSampleRate,
                                         std::string monoAnalysisPath, Score01 clippingScore,
                                         Score01 noiseScore, Score01 voicePresenceScore,
                                         std::vector<SilenceSegment> silenceMap,
                                         Score01 qualityScore,
                                         std::vector<std::string> warnings) noexcept
    : audioSourceId_{std::move(audioSourceId)}, analysisVersion_{analysisVersion},
      durationSeconds_{std::move(durationSeconds)}, analysisSampleRate_{analysisSampleRate},
      monoAnalysisPath_{std::move(monoAnalysisPath)}, clippingScore_{std::move(clippingScore)},
      noiseScore_{std::move(noiseScore)}, voicePresenceScore_{std::move(voicePresenceScore)},
      silenceMap_{std::move(silenceMap)}, qualityScore_{std::move(qualityScore)},
      warnings_{std::move(warnings)} {}

} // namespace vocalmelody::common
