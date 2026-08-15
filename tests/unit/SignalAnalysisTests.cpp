#include <vocalmelody/frontend/SignalAnalysis.h>

#include "TestContext.h"

#include <cmath>
#include <vector>

namespace {
using vocalmelody::testing::TestContext;

void testAnalyzeEmpty(TestContext& context) {
    using vocalmelody::frontend::analyzeSignal;

    const std::vector<float> empty;
    context.expect(!analyzeSignal(empty, 44100).has_value(), "an empty signal is rejected");

    const std::vector<float> singleFrame = {0.5F};
    context.expect(!analyzeSignal(singleFrame, 0).has_value(),
                   "an invalid sample rate is rejected");
}

void testAnalyzeConstant(TestContext& context) {
    using vocalmelody::frontend::analyzeSignal;

    const std::vector<float> frames(100, 0.5F);
    const auto stats = analyzeSignal(frames, 44100);
    context.expect(stats.has_value(), "a constant signal is analyzable");
    if (!stats.has_value()) {
        return;
    }

    context.expect(std::abs(stats->rms - 0.5) < 1e-6, "rms equals the constant amplitude");
    context.expect(std::abs(stats->peak - 0.5) < 1e-6, "peak equals the constant amplitude");
    context.expect(stats->clippingScore == 0.0, "a 0.5 amplitude signal is not clipped");
    context.expect(stats->silenceRatio == 0.0, "a 0.5 amplitude signal is not silent");
}

void testAnalyzeClipping(TestContext& context) {
    using vocalmelody::frontend::analyzeSignal;

    const std::vector<float> frames = {0.2F, 1.0F, 0.3F, 1.2F};
    const auto stats = analyzeSignal(frames, 44100);
    context.expect(stats.has_value(), "a clipped signal is analyzable");
    if (!stats.has_value()) {
        return;
    }

    context.expect(std::abs(stats->clippingScore - 0.5) < 1e-6,
                   "clipping score equals the clipped frame ratio");
    context.expect(std::abs(stats->peak - 1.2) < 1e-6, "peak captures the clipped amplitude");
}

void testAnalyzeSilence(TestContext& context) {
    using vocalmelody::frontend::analyzeSignal;

    const std::vector<float> frames(50, 0.0001F);
    const auto stats = analyzeSignal(frames, 44100);
    context.expect(stats.has_value(), "a silent signal is analyzable");
    if (!stats.has_value()) {
        return;
    }

    context.expect(stats->silenceRatio == 1.0, "a silent signal has a silence ratio of one");
    context.expect(stats->clippingScore == 0.0, "a silent signal is not clipped");
}

void testDetectSilenceSegments(TestContext& context) {
    using vocalmelody::frontend::detectSilenceSegments;

    std::vector<float> frames(10, 0.5F);     // 10 frames of signal
    frames.insert(frames.end(), 5, 0.0001F); // 5 silent frames
    frames.insert(frames.end(), 3, 0.5F);    // 3 frames of signal
    frames.insert(frames.end(), 4, 0.0001F); // 4 silent frames

    const auto segments = detectSilenceSegments(frames, 10);
    context.expect(segments.size() == 2, "two distinct silence runs are detected");
    if (segments.size() == 2) {
        context.expect(std::abs(segments[0].start().value() - 1.0) < 1e-6,
                       "the first silence starts at frame 10");
        context.expect(std::abs(segments[0].end().value() - 1.5) < 1e-6,
                       "the first silence ends at frame 15");
        context.expect(std::abs(segments[1].start().value() - 1.8) < 1e-6,
                       "the second silence starts at frame 18");
        context.expect(std::abs(segments[1].end().value() - 2.2) < 1e-6,
                       "the second silence ends at frame 22");
    }
}

void testEstimateNoiseFloor(TestContext& context) {
    using vocalmelody::frontend::estimateNoiseFloor;

    const std::vector<float> frames = {0.5F, 0.0005F, 0.0005F, 0.5F};
    const auto noise = estimateNoiseFloor(frames);
    context.expect(noise.has_value(), "a signal with silent frames yields a noise floor");
    if (noise.has_value()) {
        context.expect(std::abs(static_cast<double>(*noise) - 0.0005) < 1e-6,
                       "the noise floor equals the silent frame amplitude");
    }

    const std::vector<float> loudFrames(10, 0.5F);
    context.expect(!estimateNoiseFloor(loudFrames).has_value(),
                   "a signal without silent frames has no noise floor");
}

void testDownmix(TestContext& context) {
    using vocalmelody::frontend::downmixToMono;

    const std::vector<float> left = {0.2F, 0.4F};
    const std::vector<float> right = {0.4F, 0.6F};
    const auto mono = downmixToMono(left, right);
    context.expect(mono.size() == 2, "downmix keeps the channel length");
    if (mono.size() == 2) {
        context.expect(std::abs(mono[0] - 0.3F) < 1e-6F, "downmix averages the first sample");
        context.expect(std::abs(mono[1] - 0.5F) < 1e-6F, "downmix averages the second sample");
    }

    const std::vector<float> shortRight = {0.4F};
    context.expect(downmixToMono(left, shortRight).empty(),
                   "mismatched channel lengths are rejected");
}

void testResampleLinear(TestContext& context) {
    using vocalmelody::frontend::resampleLinear;

    const std::vector<float> input = {0.0F, 1.0F};
    const auto upsampled = resampleLinear(input, 2, 4);
    context.expect(upsampled.has_value() && upsampled->size() == 4,
                   "upsampling produces the duration-equivalent frame count");
    if (upsampled.has_value() && upsampled->size() == 4) {
        context.expect(std::abs((*upsampled)[0] - 0.0F) < 1e-6F,
                       "upsampling preserves the first frame");
        context.expect(std::abs((*upsampled)[1] - 0.5F) < 1e-6F,
                       "upsampling interpolates between frames");
        context.expect(std::abs((*upsampled)[2] - 1.0F) < 1e-6F,
                       "upsampling reaches the second frame");
    }

    const auto unchanged = resampleLinear(input, 16000, 16000);
    context.expect(unchanged.has_value() && *unchanged == input,
                   "equal sample rates preserve the input frames");
    context.expect(!resampleLinear({}, 44100, 16000).has_value(),
                   "an empty signal cannot be resampled");
    context.expect(!resampleLinear(input, 0, 16000).has_value(),
                   "an invalid source sample rate is rejected");
    context.expect(!resampleLinear(input, 44100, 0).has_value(),
                   "an invalid target sample rate is rejected");
}

} // namespace

int main() {
    TestContext context;
    testAnalyzeEmpty(context);
    testAnalyzeConstant(context);
    testAnalyzeClipping(context);
    testAnalyzeSilence(context);
    testDetectSilenceSegments(context);
    testEstimateNoiseFloor(context);
    testDownmix(context);
    testResampleLinear(context);
    return context.result();
}
