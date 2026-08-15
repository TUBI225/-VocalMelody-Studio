#include <vocalmelody/common/Pitch.h>

#include <cmath>
#include <utility>

namespace vocalmelody::common {

double frequencyHzToMidi(const double frequencyHz) noexcept {
    if (!std::isfinite(frequencyHz) || frequencyHz <= 0.0) {
        return 0.0;
    }
    return 69.0 + 12.0 * std::log2(frequencyHz / 440.0);
}

std::optional<PitchFrame> PitchFrame::create(const double timeSeconds, const double frequencyHz,
                                             const double midiFloat, const double confidence,
                                             const double voicedProbability,
                                             std::string estimatorId) noexcept {
    const auto time = Seconds::fromValue(timeSeconds);
    const auto midi = MidiPitch::fromValue(midiFloat);
    const auto confidence01 = Score01::fromValue(confidence);
    const auto voiced = Probability::fromValue(voicedProbability);
    if (!time.has_value() || !midi.has_value() || !confidence01.has_value() ||
        !voiced.has_value() || !std::isfinite(frequencyHz) || frequencyHz <= 0.0 ||
        estimatorId.empty()) {
        return std::nullopt;
    }

    return PitchFrame{*time, frequencyHz, *midi, *confidence01, *voiced, std::move(estimatorId)};
}

PitchFrame::PitchFrame(Seconds timeSeconds, const double frequencyHz, MidiPitch midiFloat,
                       Score01 confidence, Probability voicedProbability,
                       std::string estimatorId) noexcept
    : timeSeconds_{std::move(timeSeconds)}, frequencyHz_{frequencyHz},
      midiFloat_{std::move(midiFloat)}, confidence_{std::move(confidence)},
      voicedProbability_{std::move(voicedProbability)}, estimatorId_{std::move(estimatorId)} {}

std::optional<PitchCandidate> PitchCandidate::create(const double midiFloat,
                                                     const double frequencyHz,
                                                     const double probability,
                                                     std::vector<std::string> sourceEstimators,
                                                     const bool octaveAmbiguity) noexcept {
    const auto midi = MidiPitch::fromValue(midiFloat);
    const auto prob = Probability::fromValue(probability);
    if (!midi.has_value() || !prob.has_value() || !std::isfinite(frequencyHz) ||
        frequencyHz <= 0.0) {
        return std::nullopt;
    }

    return PitchCandidate{*midi, frequencyHz, *prob, std::move(sourceEstimators), octaveAmbiguity};
}

PitchCandidate::PitchCandidate(MidiPitch midiFloat, const double frequencyHz,
                               Probability probability, std::vector<std::string> sourceEstimators,
                               const bool octaveAmbiguity) noexcept
    : midiFloat_{std::move(midiFloat)}, frequencyHz_{frequencyHz},
      probability_{std::move(probability)}, sourceEstimators_{std::move(sourceEstimators)},
      octaveAmbiguity_{octaveAmbiguity} {}

std::optional<PitchDistributionFrame>
PitchDistributionFrame::create(const double timeSeconds, std::vector<PitchCandidate> candidates,
                               const double fusedConfidence,
                               const double voicedProbability) noexcept {
    const auto time = Seconds::fromValue(timeSeconds);
    const auto confidence = Score01::fromValue(fusedConfidence);
    const auto voiced = Probability::fromValue(voicedProbability);
    if (!time.has_value() || !confidence.has_value() || !voiced.has_value()) {
        return std::nullopt;
    }

    return PitchDistributionFrame{*time, std::move(candidates), *confidence, *voiced};
}

PitchDistributionFrame::PitchDistributionFrame(Seconds timeSeconds,
                                               std::vector<PitchCandidate> candidates,
                                               Score01 fusedConfidence,
                                               Probability voicedProbability) noexcept
    : timeSeconds_{std::move(timeSeconds)}, candidates_{std::move(candidates)},
      fusedConfidence_{std::move(fusedConfidence)},
      voicedProbability_{std::move(voicedProbability)} {}

} // namespace vocalmelody::common
