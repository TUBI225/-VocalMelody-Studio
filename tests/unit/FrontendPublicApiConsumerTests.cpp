#include <vocalmelody/frontend/SignalAnalysis.h>

#include <array>

int main() {
    constexpr std::array<float, 2> samples{0.0F, 0.1F};
    return vocalmelody::frontend::analyzeSignal(samples, 16'000).has_value() ? 0 : 1;
}
