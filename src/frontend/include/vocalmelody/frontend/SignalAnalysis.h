#pragma once

#include <vocalmelody/common/AudioAnalysisResult.h>

#include <optional>
#include <span>
#include <vector>

namespace vocalmelody::frontend {

struct SignalStats final {
    double rms{0.0};
    double peak{0.0};
    double clippingScore{0.0};
    double silenceRatio{0.0};
};

[[nodiscard]] std::optional<SignalStats> analyzeSignal(const std::span<const float> monoFrames,
                                                       const int sampleRate) noexcept;

[[nodiscard]] std::vector<common::SilenceSegment>
detectSilenceSegments(const std::span<const float> monoFrames, const int sampleRate);

[[nodiscard]] std::optional<float>
estimateNoiseFloor(const std::span<const float> monoFrames) noexcept;

[[nodiscard]] std::vector<float> downmixToMono(const std::span<const float> left,
                                               const std::span<const float> right);

[[nodiscard]] std::optional<std::vector<float>>
resampleLinear(const std::span<const float> input, int sourceSampleRate, int targetSampleRate);

} // namespace vocalmelody::frontend
