#include <vocalmelody/common/Pitch.h>
#include <vocalmelody/common/StrongTypes.h>

#include "TestContext.h"

#include <cmath>
#include <limits>

namespace {
using vocalmelody::testing::TestContext;

void testFrequencyToMidi(TestContext& context) {
    context.expect(std::abs(vocalmelody::common::frequencyHzToMidi(440.0) - 69.0) < 1e-9,
                   "A4 at 440 Hz maps to MIDI 69");
    context.expect(vocalmelody::common::frequencyHzToMidi(0.0) == 0.0,
                   "non-positive frequency maps to zero");
    context.expect(
        vocalmelody::common::frequencyHzToMidi(std::numeric_limits<double>::quiet_NaN()) == 0.0,
        "NaN frequency maps to zero");
    context.expect(
        vocalmelody::common::frequencyHzToMidi(std::numeric_limits<double>::infinity()) == 0.0,
        "infinite frequency maps to zero");
}

void testPitchFrame(TestContext& context) {
    using vocalmelody::common::PitchFrame;

    const auto frame = PitchFrame::create(0.0, 440.0, 69.0, 0.9, 0.8, "autocorrelation");
    context.expect(frame.has_value(), "a valid pitch frame is created");
    if (!frame.has_value()) {
        return;
    }

    context.expect(frame->frequencyHz() == 440.0, "frequency is preserved");
    context.expect(std::abs(frame->midiFloat().value() - 69.0) < 1e-9, "midi float is preserved");
    context.expect(frame->estimatorId() == "autocorrelation", "estimator id is preserved");

    context.expect(!PitchFrame::create(-1.0, 440.0, 69.0, 0.9, 0.8, "x").has_value(),
                   "negative time is rejected");
    context.expect(!PitchFrame::create(0.0, 0.0, 69.0, 0.9, 0.8, "x").has_value(),
                   "non-positive frequency is rejected");
    context.expect(!PitchFrame::create(0.0, 440.0, 69.0, 1.5, 0.8, "x").has_value(),
                   "confidence above one is rejected");
    context.expect(!PitchFrame::create(0.0, 440.0, 69.0, 0.9, -0.2, "x").has_value(),
                   "negative voiced probability is rejected");
    context.expect(!PitchFrame::create(0.0, 440.0, 69.0, 0.9, 0.8, "").has_value(),
                   "empty estimator id is rejected");
}

void testPitchCandidate(TestContext& context) {
    using vocalmelody::common::PitchCandidate;

    const auto candidate = PitchCandidate::create(69.0, 440.0, 0.7, {"autocorrelation"}, false);
    context.expect(candidate.has_value(), "a valid pitch candidate is created");
    if (!candidate.has_value()) {
        return;
    }

    context.expect(candidate->probability().value() == 0.7, "probability is preserved");
    context.expect(!candidate->octaveAmbiguity(), "octave ambiguity flag is preserved");

    context.expect(!PitchCandidate::create(69.0, 440.0, 1.2, {}, false).has_value(),
                   "probability above one is rejected");
    context.expect(!PitchCandidate::create(69.0, -5.0, 0.5, {}, false).has_value(),
                   "negative frequency is rejected");
}

void testPitchDistributionFrame(TestContext& context) {
    using vocalmelody::common::PitchCandidate;
    using vocalmelody::common::PitchDistributionFrame;

    const auto candidate = PitchCandidate::create(69.0, 440.0, 0.8, {}, false);
    context.expect(candidate.has_value(), "candidate for distribution is created");
    if (!candidate.has_value()) {
        return;
    }

    const auto distribution = PitchDistributionFrame::create(0.25, {*candidate}, 0.8, 0.9);
    context.expect(distribution.has_value(), "a valid distribution frame is created");
    if (!distribution.has_value()) {
        return;
    }

    context.expect(distribution->candidates().size() == 1, "candidates are preserved");
    context.expect(std::abs(distribution->timeSeconds().value() - 0.25) < 1e-9,
                   "time is preserved");

    context.expect(!PitchDistributionFrame::create(0.0, {}, -0.1, 0.5).has_value(),
                   "negative fused confidence is rejected");
}

void testPitchStrongTypes(TestContext& context) {
    using vocalmelody::common::Cents;
    using vocalmelody::common::MidiPitch;

    const auto midi = MidiPitch::fromValue(69.0);
    context.expect(midi.has_value() && midi->value() == 69.0, "MidiPitch accepts a valid value");
    context.expect(!MidiPitch::fromValue(-1.0).has_value(), "MidiPitch rejects negatives");

    const auto cents = Cents::fromValue(-65.0);
    context.expect(cents.has_value() && cents->value() == -65.0,
                   "Cents accepts a negative deviation");
    context.expect(!Cents::fromValue(std::numeric_limits<double>::infinity()).has_value(),
                   "Cents rejects infinity");
}

} // namespace

int main() {
    TestContext context;
    testFrequencyToMidi(context);
    testPitchFrame(context);
    testPitchCandidate(context);
    testPitchDistributionFrame(context);
    testPitchStrongTypes(context);
    return context.result();
}
