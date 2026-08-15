#pragma once

#include <vocalmelody/common/Pitch.h>

#include <string>
#include <string_view>
#include <vector>

namespace vocalmelody::pitch {

struct MonoSignal final {
    std::vector<float> samples;
    int sampleRate{0};
};

class IPitchEstimator {
  public:
    virtual ~IPitchEstimator() = default;

    [[nodiscard]] virtual std::vector<common::PitchFrame>
    estimate(const MonoSignal& signal) const = 0;

    [[nodiscard]] virtual std::string_view id() const noexcept = 0;
};

} // namespace vocalmelody::pitch
