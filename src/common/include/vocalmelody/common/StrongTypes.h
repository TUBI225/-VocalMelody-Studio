#pragma once

#include <cmath>
#include <compare>
#include <optional>

namespace vocalmelody::common {

template <typename Tag> class UnitInterval final {
  public:
    [[nodiscard]] static std::optional<UnitInterval> fromValue(const double value) noexcept {
        if (!std::isfinite(value) || value < 0.0 || value > 1.0) {
            return std::nullopt;
        }

        return UnitInterval{value};
    }

    [[nodiscard]] constexpr double value() const noexcept { return value_; }

    constexpr auto operator<=>(const UnitInterval&) const noexcept = default;

  private:
    explicit constexpr UnitInterval(const double value) noexcept : value_{value} {}

    double value_;
};

template <typename Tag> class NonNegativeValue final {
  public:
    [[nodiscard]] static std::optional<NonNegativeValue> fromValue(const double value) noexcept {
        if (!std::isfinite(value) || value < 0.0) {
            return std::nullopt;
        }

        return NonNegativeValue{value};
    }

    [[nodiscard]] constexpr double value() const noexcept { return value_; }

    constexpr auto operator<=>(const NonNegativeValue&) const noexcept = default;

  private:
    explicit constexpr NonNegativeValue(const double value) noexcept : value_{value} {}

    double value_;
};

template <typename Tag> class FiniteValue final {
  public:
    [[nodiscard]] static std::optional<FiniteValue> fromValue(const double value) noexcept {
        if (!std::isfinite(value)) {
            return std::nullopt;
        }

        return FiniteValue{value};
    }

    [[nodiscard]] constexpr double value() const noexcept { return value_; }

    constexpr auto operator<=>(const FiniteValue&) const noexcept = default;

  private:
    explicit constexpr FiniteValue(const double value) noexcept : value_{value} {}

    double value_;
};

struct ProbabilityTag final {};
struct Score01Tag final {};
struct SecondsTag final {};
struct BeatsTag final {};
struct MidiPitchTag final {};
struct CentsTag final {};

using Probability = UnitInterval<ProbabilityTag>;
using Score01 = UnitInterval<Score01Tag>;
using Seconds = NonNegativeValue<SecondsTag>;
using Beats = NonNegativeValue<BeatsTag>;
using MidiPitch = NonNegativeValue<MidiPitchTag>;
using Cents = FiniteValue<CentsTag>;

} // namespace vocalmelody::common
