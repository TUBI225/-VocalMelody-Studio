#pragma once

#include <vocalmelody/common/StrongTypes.h>

#include <optional>
#include <string>
#include <vector>

namespace vocalmelody::common {

[[nodiscard]] double frequencyHzToMidi(const double frequencyHz) noexcept;

class PitchFrame final {
  public:
    [[nodiscard]] static std::optional<PitchFrame> create(double timeSeconds, double frequencyHz,
                                                          double midiFloat, double confidence,
                                                          double voicedProbability,
                                                          std::string estimatorId) noexcept;

    [[nodiscard]] const Seconds& timeSeconds() const noexcept { return timeSeconds_; }
    [[nodiscard]] double frequencyHz() const noexcept { return frequencyHz_; }
    [[nodiscard]] const MidiPitch& midiFloat() const noexcept { return midiFloat_; }
    [[nodiscard]] const Score01& confidence() const noexcept { return confidence_; }
    [[nodiscard]] const Probability& voicedProbability() const noexcept {
        return voicedProbability_;
    }
    [[nodiscard]] const std::string& estimatorId() const noexcept { return estimatorId_; }

  private:
    PitchFrame(Seconds timeSeconds, double frequencyHz, MidiPitch midiFloat, Score01 confidence,
               Probability voicedProbability, std::string estimatorId) noexcept;

    Seconds timeSeconds_;
    double frequencyHz_;
    MidiPitch midiFloat_;
    Score01 confidence_;
    Probability voicedProbability_;
    std::string estimatorId_;
};

class PitchCandidate final {
  public:
    [[nodiscard]] static std::optional<PitchCandidate>
    create(double midiFloat, double frequencyHz, double probability,
           std::vector<std::string> sourceEstimators, bool octaveAmbiguity) noexcept;

    [[nodiscard]] const MidiPitch& midiFloat() const noexcept { return midiFloat_; }
    [[nodiscard]] double frequencyHz() const noexcept { return frequencyHz_; }
    [[nodiscard]] const Probability& probability() const noexcept { return probability_; }
    [[nodiscard]] const std::vector<std::string>& sourceEstimators() const noexcept {
        return sourceEstimators_;
    }
    [[nodiscard]] bool octaveAmbiguity() const noexcept { return octaveAmbiguity_; }

  private:
    PitchCandidate(MidiPitch midiFloat, double frequencyHz, Probability probability,
                   std::vector<std::string> sourceEstimators, bool octaveAmbiguity) noexcept;

    MidiPitch midiFloat_;
    double frequencyHz_;
    Probability probability_;
    std::vector<std::string> sourceEstimators_;
    bool octaveAmbiguity_;
};

class PitchDistributionFrame final {
  public:
    [[nodiscard]] static std::optional<PitchDistributionFrame>
    create(double timeSeconds, std::vector<PitchCandidate> candidates, double fusedConfidence,
           double voicedProbability) noexcept;

    [[nodiscard]] const Seconds& timeSeconds() const noexcept { return timeSeconds_; }
    [[nodiscard]] const std::vector<PitchCandidate>& candidates() const noexcept {
        return candidates_;
    }
    [[nodiscard]] const Score01& fusedConfidence() const noexcept { return fusedConfidence_; }
    [[nodiscard]] const Probability& voicedProbability() const noexcept {
        return voicedProbability_;
    }

  private:
    PitchDistributionFrame(Seconds timeSeconds, std::vector<PitchCandidate> candidates,
                           Score01 fusedConfidence, Probability voicedProbability) noexcept;

    Seconds timeSeconds_;
    std::vector<PitchCandidate> candidates_;
    Score01 fusedConfidence_;
    Probability voicedProbability_;
};

} // namespace vocalmelody::common
