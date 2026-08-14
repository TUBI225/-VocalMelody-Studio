#pragma once

#include <iostream>
#include <string_view>

namespace vocalmelody::testing {

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

} // namespace vocalmelody::testing
