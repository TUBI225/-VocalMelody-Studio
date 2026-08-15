#pragma once

#include "WavTestHelpers.h"

#include <cstdint>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace vocalmelody::testing {

#ifndef VOCALMELODY_MP3_TEST_VECTOR
#error "VOCALMELODY_MP3_TEST_VECTOR must point to the pinned minimp3 Layer III vector"
#endif

inline std::string mp3TestVectorPath() { return VOCALMELODY_MP3_TEST_VECTOR; }

inline std::vector<std::uint8_t> readMp3TestVector() {
    std::ifstream file(mp3TestVectorPath(), std::ios::binary);
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

inline void copyMp3TestVector(const std::string& path) {
    const auto data = readMp3TestVector();
    std::ofstream file(path, std::ios::binary);
    file.write(reinterpret_cast<const char*>(data.data()),
               static_cast<std::streamsize>(data.size()));
}

} // namespace vocalmelody::testing
