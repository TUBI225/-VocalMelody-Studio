#include <vocalmelody/frontend/SignalAnalysis.h>

#include <algorithm>
#include <cmath>

namespace vocalmelody::frontend {
namespace {

constexpr float kSilenceAmplitude = 0.001F;
constexpr float kClippingAmplitude = 0.99F;
constexpr std::size_t kMinSilenceRun = 2;

[[nodiscard]] float square(const float value) noexcept { return value * value; }

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
                                                          const int sampleRate) noexcept {
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
                                 const std::span<const float> right) noexcept {
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

} // namespace vocalmelody::frontend
