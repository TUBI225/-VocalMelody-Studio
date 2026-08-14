#include <vocalmelody/common/StrongTypes.h>

#include <cmath>
#include <iostream>
#include <limits>
#include <string_view>

namespace {

class TestContext final {
  public:
    void expect(const bool condition, const std::string_view message) {
        if (!condition) {
            ++failures_;
            std::cerr << "FAILED: " << message << '\n';
        }
    }

    [[nodiscard]] int result() const noexcept { return failures_ == 0 ? 0 : 1; }

  private:
    int failures_{0};
};

void testUnitIntervals(TestContext& context) {
    using vocalmelody::common::Probability;

    const auto zero = Probability::fromValue(0.0);
    const auto middle = Probability::fromValue(0.42);
    const auto one = Probability::fromValue(1.0);

    context.expect(zero.has_value() && zero->value() == 0.0,
                   "Probability accepts the lower boundary");
    context.expect(middle.has_value() && middle->value() == 0.42,
                   "Probability preserves a valid value");
    context.expect(one.has_value() && one->value() == 1.0,
                   "Probability accepts the upper boundary");
    context.expect(!Probability::fromValue(-0.01).has_value(),
                   "Probability rejects values below zero");
    context.expect(!Probability::fromValue(1.01).has_value(),
                   "Probability rejects values above one");
    context.expect(!Probability::fromValue(std::numeric_limits<double>::quiet_NaN()).has_value(),
                   "Probability rejects NaN");
    context.expect(!Probability::fromValue(std::numeric_limits<double>::infinity()).has_value(),
                   "Probability rejects infinity");
}

void testNonNegativeTime(TestContext& context) {
    using vocalmelody::common::Seconds;

    const auto start = Seconds::fromValue(0.0);
    const auto eventTime = Seconds::fromValue(1.843);

    context.expect(start.has_value() && start->value() == 0.0,
                   "Seconds accepts the beginning of the timeline");
    context.expect(eventTime.has_value() && eventTime->value() == 1.843,
                   "Seconds preserves performance time");
    context.expect(!Seconds::fromValue(-0.001).has_value(),
                   "Seconds rejects negative performance time");
    context.expect(!Seconds::fromValue(std::numeric_limits<double>::quiet_NaN()).has_value(),
                   "Seconds rejects NaN");
}

void testScore01(TestContext& context) {
    using vocalmelody::common::Score01;

    context.expect(Score01::fromValue(0.5).has_value(), "Score01 accepts a valid score");
    context.expect(!Score01::fromValue(1.5).has_value(), "Score01 rejects scores above one");
    context.expect(!Score01::fromValue(-0.5).has_value(), "Score01 rejects negative scores");
    context.expect(!Score01::fromValue(std::numeric_limits<double>::quiet_NaN()).has_value(),
                   "Score01 rejects NaN");
}

void testBeats(TestContext& context) {
    using vocalmelody::common::Beats;

    const auto beats = Beats::fromValue(4.0);
    context.expect(beats.has_value() && beats->value() == 4.0, "Beats accepts a valid beat count");
    context.expect(!Beats::fromValue(-1.0).has_value(), "Beats rejects negative beat counts");
    context.expect(!Beats::fromValue(std::numeric_limits<double>::infinity()).has_value(),
                   "Beats rejects infinity");
}

void testBoundaryZero(TestContext& context) {
    using vocalmelody::common::Probability;

    const auto negativeZero = Probability::fromValue(-0.0);
    context.expect(negativeZero.has_value() && negativeZero->value() == 0.0,
                   "Probability accepts -0.0 as the lower boundary");
}

void testComparisons(TestContext& context) {
    using vocalmelody::common::Probability;

    const auto lower = Probability::fromValue(0.3);
    const auto equal = Probability::fromValue(0.3);
    const auto higher = Probability::fromValue(0.7);

    context.expect(lower.has_value() && equal.has_value() && higher.has_value(),
                   "comparison values construct");
    context.expect(*lower == *equal, "equal values compare equal");
    context.expect(*lower < *higher, "lower value orders before higher");
    context.expect(*higher > *lower, "higher value orders after lower");
    context.expect(!(*lower == *higher), "different values compare different");
}

} // namespace

int main() {
    TestContext context;
    testUnitIntervals(context);
    testNonNegativeTime(context);
    testScore01(context);
    testBeats(context);
    testBoundaryZero(context);
    testComparisons(context);
    return context.result();
}
