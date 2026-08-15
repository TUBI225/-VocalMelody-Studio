#include <vocalmelody/pitch/AutocorrelationPitchEstimator.h>
#include <vocalmelody/pitch/IPitchEstimator.h>

#include "TestContext.h"

#include <cmath>
#include <string>
#include <vector>

namespace {
using vocalmelody::testing::TestContext;

[[nodiscard]] std::vector<float> makeSine(const int sampleRate, const double frequencyHz,
                                          const double durationSeconds) {
    std::vector<float> samples;
    const int count = static_cast<int>(durationSeconds * static_cast<double>(sampleRate));
    samples.reserve(static_cast<std::size_t>(count));
    for (int i = 0; i < count; ++i) {
        const double phase = 2.0 * 3.14159265358979323846 * frequencyHz * static_cast<double>(i) /
                             static_cast<double>(sampleRate);
        samples.push_back(static_cast<float>(std::sin(phase) * 0.5));
    }
    return samples;
}

void testInterfaceId(TestContext& context) {
    using vocalmelody::pitch::AutocorrelationPitchEstimator;
    using vocalmelody::pitch::IPitchEstimator;

    const AutocorrelationPitchEstimator estimator;
    const IPitchEstimator& interface = estimator;
    context.expect(interface.id() == "autocorrelation", "the estimator exposes its id");
}

void testSine440(TestContext& context) {
    using vocalmelody::pitch::AutocorrelationPitchEstimator;
    using vocalmelody::pitch::MonoSignal;

    const AutocorrelationPitchEstimator estimator;
    const MonoSignal signal{makeSine(16000, 440.0, 1.0), 16000};
    const auto frames = estimator.estimate(signal);
    context.expect(!frames.empty(), "a sine wave produces pitch frames");
    if (frames.empty()) {
        return;
    }

    const double detectedHz = frames.front().frequencyHz();
    context.expect(std::abs(detectedHz - 440.0) < 10.0,
                   "the detected frequency matches A4 within tolerance (detected=" +
                       std::to_string(detectedHz) + " Hz)");
    context.expect(frames.front().voicedProbability().value() > 0.5, "a clean sine is voiced");
}

void testEmptySignal(TestContext& context) {
    using vocalmelody::pitch::AutocorrelationPitchEstimator;
    using vocalmelody::pitch::MonoSignal;

    const AutocorrelationPitchEstimator estimator;
    context.expect(estimator.estimate(MonoSignal{{}, 16000}).empty(),
                   "an empty signal produces no frames");
    context.expect(estimator.estimate(MonoSignal{{0.5F}, 0}).empty(),
                   "an invalid sample rate produces no frames");
}

void testInterfaceUsage(TestContext& context) {
    using vocalmelody::pitch::AutocorrelationPitchEstimator;
    using vocalmelody::pitch::IPitchEstimator;
    using vocalmelody::pitch::MonoSignal;

    const AutocorrelationPitchEstimator autocorrelation;
    const IPitchEstimator& estimator = autocorrelation;
    const MonoSignal signal{makeSine(16000, 440.0, 0.5), 16000};
    const auto frames = estimator.estimate(signal);
    context.expect(!frames.empty(), "the interface estimates frames");
    if (!frames.empty()) {
        context.expect(frames.front().estimatorId() == "autocorrelation",
                       "frames are tagged with the estimator id");
    }
}

} // namespace

int main() {
    TestContext context;
    testInterfaceId(context);
    testSine440(context);
    testEmptySignal(context);
    testInterfaceUsage(context);
    return context.result();
}
