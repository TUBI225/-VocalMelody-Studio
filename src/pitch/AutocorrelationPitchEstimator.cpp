#include <vocalmelody/pitch/AutocorrelationPitchEstimator.h>

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

namespace vocalmelody::pitch {
namespace {

constexpr double kAutocorrelationThreshold = 0.8;

[[nodiscard]] std::vector<float> extractFrame(const std::vector<float>& samples,
                                              const std::size_t start, const int frameSize) {
    std::vector<float> frame(static_cast<std::size_t>(frameSize), 0.0F);
    const std::size_t available = std::min(frame.size(), samples.size() - start);
    for (std::size_t i = 0; i < available; ++i) {
        frame[i] = samples[start + i];
    }
    return frame;
}

} // namespace

std::vector<common::PitchFrame>
AutocorrelationPitchEstimator::estimate(const MonoSignal& signal) const {
    std::vector<common::PitchFrame> frames;
    if (signal.sampleRate <= 0 || signal.samples.empty()) {
        return frames;
    }

    const double minLag = std::ceil(static_cast<double>(signal.sampleRate) / kMaxFrequencyHz);
    const double maxLag = std::floor(static_cast<double>(signal.sampleRate) / kMinFrequencyHz);
    const int minLagI = static_cast<int>(std::max(2.0, minLag));
    const int maxLagI = static_cast<int>(maxLag);
    if (maxLagI < minLagI) {
        return frames;
    }

    for (std::size_t start = 0; start < signal.samples.size(); start += kDefaultHopSize) {
        const double timeSeconds =
            static_cast<double>(start) / static_cast<double>(signal.sampleRate);
        const auto frame = extractFrame(signal.samples, start, kDefaultFrameSize);
        const auto pitch = estimateFrame(frame, signal.sampleRate, minLagI, maxLagI, timeSeconds);
        if (pitch.has_value()) {
            frames.push_back(*pitch);
        }
    }

    return frames;
}

std::optional<common::PitchFrame>
AutocorrelationPitchEstimator::estimateFrame(const std::vector<float>& frame, const int sampleRate,
                                             const int minLag, const int maxLag,
                                             const double timeSeconds) const {
    double energy = 0.0;
    for (const float sample : frame) {
        energy += static_cast<double>(sample) * static_cast<double>(sample);
    }
    if (energy <= 0.0) {
        return std::nullopt;
    }

    if (frame.size() < 3U || minLag < 2) {
        return std::nullopt;
    }
    const int effectiveMaxLag = std::min(maxLag, static_cast<int>(frame.size() - 2U));
    if (effectiveMaxLag < minLag) {
        return std::nullopt;
    }

    const int comparisonMinLag = minLag - 1;
    const int comparisonMaxLag = effectiveMaxLag + 1;
    std::vector<double> correlations(static_cast<std::size_t>(comparisonMaxLag + 1), 0.0);
    for (int lag = comparisonMinLag; lag <= comparisonMaxLag; ++lag) {
        const std::size_t lagIndex = static_cast<std::size_t>(lag);
        double sum = 0.0;
        double normCurrent = 0.0;
        double normDelayed = 0.0;
        for (std::size_t i = 0; i + lagIndex < frame.size(); ++i) {
            const double current = static_cast<double>(frame[i]);
            const double delayed = static_cast<double>(frame[i + lagIndex]);
            sum += current * delayed;
            normCurrent += current * current;
            normDelayed += delayed * delayed;
        }
        if (normCurrent > 0.0 && normDelayed > 0.0) {
            correlations[lagIndex] = sum / std::sqrt(normCurrent * normDelayed);
        }
    }

    // Select the first local peak above the voicing threshold. Using the global
    // peak instead would bias toward sub-harmonics (classic autocorrelation issue).
    int bestLag = 0;
    double bestCorrelation = 0.0;
    for (int lag = minLag; lag <= effectiveMaxLag; ++lag) {
        const std::size_t lagIndex = static_cast<std::size_t>(lag);
        const double current = correlations[lagIndex];
        const double previous = correlations[lagIndex - 1U];
        const double next = correlations[lagIndex + 1U];
        if (current >= kAutocorrelationThreshold && current > previous && current >= next) {
            bestLag = lag;
            bestCorrelation = current;
            break;
        }
    }

    if (bestLag <= 0 || bestCorrelation <= 0.0) {
        return std::nullopt;
    }

    const double frequency = static_cast<double>(sampleRate) / static_cast<double>(bestLag);
    const double confidence = std::min(1.0, std::max(0.0, bestCorrelation));
    const double voicedProbability = std::min(1.0, std::max(0.0, bestCorrelation));
    const double midi = common::frequencyHzToMidi(frequency);

    return common::PitchFrame::create(timeSeconds, frequency, midi, confidence, voicedProbability,
                                      std::string(id()));
}

} // namespace vocalmelody::pitch
