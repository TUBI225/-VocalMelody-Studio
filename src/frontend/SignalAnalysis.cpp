#include <vocalmelody/frontend/SignalAnalysis.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numeric>

namespace vocalmelody::frontend {
namespace {

constexpr float kSilenceAmplitude = 0.001F;
constexpr float kClippingAmplitude = 0.99F;
constexpr std::size_t kMinSilenceRun = 2;
constexpr std::size_t kMaxResampledFrames = 100'000'000;
constexpr int kSincRadius = 16;
constexpr std::size_t kSincTapCount = static_cast<std::size_t>(kSincRadius * 2 + 1);
constexpr std::size_t kMaxSincPhaseCount = 1024;
constexpr double kDownsampleCutoffMargin = 0.90;
constexpr double kPi = 3.14159265358979323846;

[[nodiscard]] float square(const float value) noexcept { return value * value; }

[[nodiscard]] std::optional<std::size_t> resampledFrameCount(const std::size_t inputSize,
                                                             const int sourceSampleRate,
                                                             const int targetSampleRate) noexcept {
    if (inputSize == 0 || sourceSampleRate <= 0 || targetSampleRate <= 0) {
        return std::nullopt;
    }
    if (inputSize > static_cast<std::size_t>(std::numeric_limits<std::int64_t>::max())) {
        return std::nullopt;
    }

    const long double exactOutputSize = static_cast<long double>(inputSize) *
                                        static_cast<long double>(targetSampleRate) /
                                        static_cast<long double>(sourceSampleRate);
    if (!std::isfinite(exactOutputSize) || exactOutputSize <= 0.0L ||
        exactOutputSize > static_cast<long double>(kMaxResampledFrames) ||
        exactOutputSize > static_cast<long double>(std::numeric_limits<std::size_t>::max())) {
        return std::nullopt;
    }

    return std::max<std::size_t>(1, static_cast<std::size_t>(std::llround(exactOutputSize)));
}

[[nodiscard]] double sinc(const double value) noexcept {
    if (std::abs(value) < 1.0e-12) {
        return 1.0;
    }
    const double angle = kPi * value;
    return std::sin(angle) / angle;
}

[[nodiscard]] double blackmanWindow(const double distance) noexcept {
    const double normalizedDistance = distance / static_cast<double>(kSincRadius);
    return 0.42 + 0.5 * std::cos(kPi * normalizedDistance) +
           0.08 * std::cos(2.0 * kPi * normalizedDistance);
}

[[nodiscard]] std::vector<double> makeSincKernelTable(const double cutoffScale,
                                                      const std::size_t phaseCount) {
    std::vector<double> kernels(phaseCount * kSincTapCount, 0.0);
    for (std::size_t phaseIndex = 0; phaseIndex < phaseCount; ++phaseIndex) {
        const double fraction = static_cast<double>(phaseIndex) / static_cast<double>(phaseCount);
        for (std::size_t tapIndex = 0; tapIndex < kSincTapCount; ++tapIndex) {
            const int offset = static_cast<int>(tapIndex) - kSincRadius;
            const double distance = static_cast<double>(offset) - fraction;
            if (std::abs(distance) <= static_cast<double>(kSincRadius)) {
                kernels[phaseIndex * kSincTapCount + tapIndex] =
                    cutoffScale * sinc(cutoffScale * distance) * blackmanWindow(distance);
            }
        }
    }
    return kernels;
}

} // namespace

std::optional<SignalStats> analyzeSignal(const std::span<const float> monoFrames,
                                         const int sampleRate) noexcept {
    if (monoFrames.empty() || sampleRate <= 0) {
        return std::nullopt;
    }

    double sumSquares = 0.0;
    double peak = 0.0;
    std::size_t clippedFrames = 0;
    std::size_t silentFrames = 0;

    for (const float frame : monoFrames) {
        const double magnitude = std::abs(static_cast<double>(frame));
        sumSquares += square(frame);
        peak = std::max(peak, magnitude);
        if (magnitude >= kClippingAmplitude) {
            ++clippedFrames;
        }
        if (magnitude < kSilenceAmplitude) {
            ++silentFrames;
        }
    }

    const double frameCount = static_cast<double>(monoFrames.size());
    const double rms = std::sqrt(sumSquares / frameCount);
    const double clippingScore = static_cast<double>(clippedFrames) / frameCount;
    const double silenceRatio = static_cast<double>(silentFrames) / frameCount;

    return SignalStats{rms, peak, clippingScore, silenceRatio};
}

std::vector<common::SilenceSegment> detectSilenceSegments(const std::span<const float> monoFrames,
                                                          const int sampleRate) {
    std::vector<common::SilenceSegment> segments;
    if (monoFrames.empty() || sampleRate <= 0) {
        return segments;
    }

    std::size_t runStart = 0;
    while (runStart < monoFrames.size()) {
        if (std::abs(static_cast<double>(monoFrames[runStart])) >= kSilenceAmplitude) {
            ++runStart;
            continue;
        }

        std::size_t runEnd = runStart;
        while (runEnd < monoFrames.size() &&
               std::abs(static_cast<double>(monoFrames[runEnd])) < kSilenceAmplitude) {
            ++runEnd;
        }

        if (runEnd - runStart >= kMinSilenceRun) {
            const auto startSeconds = common::Seconds::fromValue(static_cast<double>(runStart) /
                                                                 static_cast<double>(sampleRate));
            const auto endSeconds = common::Seconds::fromValue(static_cast<double>(runEnd) /
                                                               static_cast<double>(sampleRate));
            if (startSeconds.has_value() && endSeconds.has_value()) {
                if (const auto segment = common::SilenceSegment::create(*startSeconds, *endSeconds);
                    segment.has_value()) {
                    segments.push_back(*segment);
                }
            }
        }

        runStart = runEnd;
    }

    return segments;
}

std::optional<float> estimateNoiseFloor(const std::span<const float> monoFrames) noexcept {
    double sumSquares = 0.0;
    std::size_t silentFrames = 0;

    for (const float frame : monoFrames) {
        if (std::abs(static_cast<double>(frame)) < kSilenceAmplitude) {
            sumSquares += square(frame);
            ++silentFrames;
        }
    }

    if (silentFrames == 0) {
        return std::nullopt;
    }

    return static_cast<float>(std::sqrt(sumSquares / static_cast<double>(silentFrames)));
}

std::vector<float> downmixToMono(const std::span<const float> left,
                                 const std::span<const float> right) {
    if (left.size() != right.size()) {
        return {};
    }

    std::vector<float> mono;
    mono.reserve(left.size());
    for (std::size_t i = 0; i < left.size(); ++i) {
        const double average = (static_cast<double>(left[i]) + static_cast<double>(right[i])) * 0.5;
        mono.push_back(static_cast<float>(average));
    }
    return mono;
}

std::optional<std::vector<float>> resampleLinear(const std::span<const float> input,
                                                 const int sourceSampleRate,
                                                 const int targetSampleRate) {
    const auto outputSize = resampledFrameCount(input.size(), sourceSampleRate, targetSampleRate);
    if (!outputSize.has_value()) {
        return std::nullopt;
    }
    if (sourceSampleRate == targetSampleRate) {
        return std::vector<float>(input.begin(), input.end());
    }

    std::vector<float> output(*outputSize);
    const double sourceStep =
        static_cast<double>(sourceSampleRate) / static_cast<double>(targetSampleRate);

    for (std::size_t outputIndex = 0; outputIndex < *outputSize; ++outputIndex) {
        const double sourcePosition = static_cast<double>(outputIndex) * sourceStep;
        const auto lowerIndex =
            std::min(static_cast<std::size_t>(sourcePosition), input.size() - 1);
        const auto upperIndex = std::min(lowerIndex + 1, input.size() - 1);
        const double fraction = sourcePosition - static_cast<double>(lowerIndex);
        output[outputIndex] =
            static_cast<float>(static_cast<double>(input[lowerIndex]) * (1.0 - fraction) +
                               static_cast<double>(input[upperIndex]) * fraction);
    }

    return output;
}

std::optional<std::vector<float>> resampleWindowedSinc(const std::span<const float> input,
                                                       const int sourceSampleRate,
                                                       const int targetSampleRate) {
    const auto outputSize = resampledFrameCount(input.size(), sourceSampleRate, targetSampleRate);
    if (!outputSize.has_value()) {
        return std::nullopt;
    }
    if (sourceSampleRate == targetSampleRate) {
        return std::vector<float>(input.begin(), input.end());
    }

    const double rateRatio =
        static_cast<double>(targetSampleRate) / static_cast<double>(sourceSampleRate);
    const double cutoffScale = rateRatio < 1.0 ? rateRatio * kDownsampleCutoffMargin : 1.0;
    const double sourceStep = 1.0 / rateRatio;
    const int rateDivisor = std::gcd(sourceSampleRate, targetSampleRate);
    const auto exactPhaseCount = static_cast<std::size_t>(targetSampleRate / rateDivisor);
    const auto phaseCount = std::min(kMaxSincPhaseCount, exactPhaseCount);
    const auto kernels = makeSincKernelTable(cutoffScale, phaseCount);

    std::vector<float> output(*outputSize, 0.0F);
    for (std::size_t outputIndex = 0; outputIndex < *outputSize; ++outputIndex) {
        const double sourcePosition = static_cast<double>(outputIndex) * sourceStep;
        const auto centerIndex = static_cast<std::int64_t>(std::floor(sourcePosition));
        const double fraction = sourcePosition - static_cast<double>(centerIndex);
        const auto phaseIndex = std::min(
            phaseCount - 1U,
            static_cast<std::size_t>(std::llround(fraction * static_cast<double>(phaseCount))));
        const auto firstIndex = centerIndex - kSincRadius;

        double weightedSum = 0.0;
        double weightSum = 0.0;
        for (std::size_t tapIndex = 0; tapIndex < kSincTapCount; ++tapIndex) {
            const auto sourceIndex = firstIndex + static_cast<std::int64_t>(tapIndex);
            if (sourceIndex < 0 || sourceIndex >= static_cast<std::int64_t>(input.size())) {
                continue;
            }

            const double weight = kernels[phaseIndex * kSincTapCount + tapIndex];
            weightedSum +=
                static_cast<double>(input[static_cast<std::size_t>(sourceIndex)]) * weight;
            weightSum += weight;
        }

        if (!std::isfinite(weightedSum) || !std::isfinite(weightSum) ||
            std::abs(weightSum) < 1.0e-12) {
            return std::nullopt;
        }
        output[outputIndex] = static_cast<float>(weightedSum / weightSum);
    }

    return output;
}

} // namespace vocalmelody::frontend
