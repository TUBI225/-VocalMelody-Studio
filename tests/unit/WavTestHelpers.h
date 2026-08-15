#pragma once

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

namespace vocalmelody::testing {

inline void writeInt16(std::ofstream& file, const int16_t value) {
    const auto bytes = static_cast<uint16_t>(value);
    file.put(static_cast<char>(bytes & 0xFF));
    file.put(static_cast<char>((bytes >> 8) & 0xFF));
}

inline void writeInt32(std::ofstream& file, const int32_t value) {
    const auto bytes = static_cast<uint32_t>(value);
    file.put(static_cast<char>(bytes & 0xFF));
    file.put(static_cast<char>((bytes >> 8) & 0xFF));
    file.put(static_cast<char>((bytes >> 16) & 0xFF));
    file.put(static_cast<char>((bytes >> 24) & 0xFF));
}

inline void writePcm16Wav(const std::string& path, const int sampleRate, const int channels,
                          const std::vector<std::vector<float>>& samples) {
    const int numFrames = static_cast<int>(samples[0].size());
    constexpr int bytesPerSample = 2;
    const int blockAlign = channels * bytesPerSample;
    const int dataSize = numFrames * blockAlign;
    const int byteRate = sampleRate * blockAlign;

    std::ofstream file(path, std::ios::binary);
    file.write("RIFF", 4);
    writeInt32(file, 36 + dataSize);
    file.write("WAVE", 4);
    file.write("fmt ", 4);
    writeInt32(file, 16);
    writeInt16(file, 1);
    writeInt16(file, static_cast<int16_t>(channels));
    writeInt32(file, sampleRate);
    writeInt32(file, byteRate);
    writeInt16(file, static_cast<int16_t>(blockAlign));
    writeInt16(file, 16);
    file.write("data", 4);
    writeInt32(file, dataSize);
    for (int frame = 0; frame < numFrames; ++frame) {
        for (int ch = 0; ch < channels; ++ch) {
            const float value =
                samples[static_cast<std::size_t>(ch)][static_cast<std::size_t>(frame)];
            const float clamped = std::max(-1.0F, std::min(1.0F, value));
            const auto sample = static_cast<int16_t>(clamped * 32767.0F);
            writeInt16(file, sample);
        }
    }
}

inline std::string tempFilePath(const std::string& name) { return std::string(".\\") + name; }

inline std::string fnv1aFileHash(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return {};
    }

    constexpr uint64_t offsetBasis = 14695981039346656037ULL;
    constexpr uint64_t prime = 1099511628211ULL;
    uint64_t hash = offsetBasis;
    char buffer[4096]{};
    while (file.read(buffer, static_cast<std::streamsize>(sizeof(buffer)))) {
        const auto count = file.gcount();
        for (std::streamsize i = 0; i < count; ++i) {
            hash ^= static_cast<uint64_t>(
                static_cast<unsigned char>(buffer[static_cast<std::size_t>(i)]));
            hash *= prime;
        }
    }
    const auto remaining = file.gcount();
    for (std::streamsize i = 0; i < remaining; ++i) {
        hash ^=
            static_cast<uint64_t>(static_cast<unsigned char>(buffer[static_cast<std::size_t>(i)]));
        hash *= prime;
    }

    char hex[17]{};
    std::snprintf(hex, sizeof(hex), "%016llx", static_cast<unsigned long long>(hash));
    return std::string(hex);
}

} // namespace vocalmelody::testing
