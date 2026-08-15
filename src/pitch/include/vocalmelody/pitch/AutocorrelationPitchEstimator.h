#pragma once

#include <vocalmelody/pitch/IPitchEstimator.h>

#include <optional>

namespace vocalmelody::pitch {

class AutocorrelationPitchEstimator final : public IPitchEstimator {
  public:
    static constexpr int kDefaultFrameSize = 2048;
    static constexpr int kDefaultHopSize = 1024;
    static constexpr double kMinFrequencyHz = 80.0;
    static constexpr double kMaxFrequencyHz = 2000.0;

    [[nodiscard]] std::vector<common::PitchFrame> estimate(const MonoSignal& signal) const override;

    [[nodiscard]] std::string_view id() const noexcept override { return "autocorrelation"; }

  private:
    [[nodiscard]] std::optional<common::PitchFrame> estimateFrame(const std::vector<float>& frame,
                                                                  int sampleRate, int minLag,
                                                                  int maxLag,
                                                                  double timeSeconds) const;
};

} // namespace vocalmelody::pitch
