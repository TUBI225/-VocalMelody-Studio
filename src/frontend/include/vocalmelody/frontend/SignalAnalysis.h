#pragma once

#include <vocalmelody/common/AudioAnalysisResult.h>

#include <functional>
#include <optional>
#include <span>
#include <stop_token>
#include <vector>

namespace vocalmelody::frontend {

struct SignalStats final {
    double rms{0.0};
    double peak{0.0};
    double clippingScore{0.0};
    double silenceRatio{0.0};
};

using ProcessingProgressCallback = std::function<void(double)>;

[[nodiscard]] std::optional<SignalStats> analyzeSignal(const std::span<const float> monoFrames,
                                                       int sampleRate,
                                                       std::stop_token stopToken = {}) noexcept;

[[nodiscard]] std::vector<common::SilenceSegment>
detectSilenceSegments(const std::span<const float> monoFrames, int sampleRate,
                      std::stop_token stopToken = {});

[[nodiscard]] std::optional<float> estimateNoiseFloor(const std::span<const float> monoFrames,
                                                      std::stop_token stopToken = {}) noexcept;

[[nodiscard]] std::vector<float> downmixToMono(const std::span<const float> left,
                                               const std::span<const float> right);

[[nodiscard]] std::optional<std::vector<float>>
resampleLinear(const std::span<const float> input, int sourceSampleRate, int targetSampleRate);

[[nodiscard]] std::optional<std::vector<float>>
resampleWindowedSinc(const std::span<const float> input, int sourceSampleRate, int targetSampleRate,
                     std::stop_token stopToken = {},
                     ProcessingProgressCallback progressCallback = {});

} // namespace vocalmelody::frontend
