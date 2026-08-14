#pragma once

#include <string_view>

namespace vocalmelody::common {

[[nodiscard]] inline constexpr std::string_view applicationName() noexcept {
    return "VocalMelody Studio";
}

[[nodiscard]] inline constexpr std::string_view applicationVersion() noexcept { return "0.1.0"; }

} // namespace vocalmelody::common
