#include <vocalmelody/audio/Mp3Decoder.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244 4267 4456 4701 4706)
#endif

#define MINIMP3_IMPLEMENTATION
#include <minimp3_ex.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <algorithm>
#include <filesystem>

namespace vocalmelody::audio {
namespace {

constexpr float kInt16ToFloatNorm = 1.0F / 32768.0F;

class DecoderGuard final {
  public:
    explicit DecoderGuard(mp3dec_ex_t& decoder) noexcept : decoder_{decoder} {}
    ~DecoderGuard() { mp3dec_ex_close(&decoder_); }

    DecoderGuard(const DecoderGuard&) = delete;
    DecoderGuard& operator=(const DecoderGuard&) = delete;

  private:
    mp3dec_ex_t& decoder_;
};

[[nodiscard]] std::optional<DecodedAudioData> processDecoder(mp3dec_ex_t& dec) noexcept {
    DecoderGuard guard(dec);
    try {
        if (dec.info.channels < 1 || dec.info.channels > 2 || dec.info.hz <= 0 ||
            dec.samples == 0) {
            return std::nullopt;
        }

        const auto sampleRate = dec.info.hz;
        const auto channelCount = dec.info.channels;
        const auto channels = static_cast<std::size_t>(channelCount);
        const auto totalSamples = dec.samples;
        if (totalSamples % channels != 0 ||
            totalSamples > Mp3Decoder::kMaxDecodedFrames * channels) {
            return std::nullopt;
        }
        const auto totalFrames = totalSamples / channels;

        if (totalFrames == 0 || totalFrames > Mp3Decoder::kMaxDecodedFrames) {
            return std::nullopt;
        }

        DecodedAudioData result;
        result.sampleRate = sampleRate;
        result.channelCount = channelCount;
        result.bitDepth = 16;
        result.totalFrames = static_cast<std::int64_t>(totalFrames);
        result.monoSamples.reserve(totalFrames);

        std::size_t decodedSamples = 0;
        while (decodedSamples < totalSamples) {
            mp3d_sample_t* framePcm = nullptr;
            mp3dec_frame_info_t frameInfo{};
            const auto frameSamples =
                mp3dec_ex_read_frame(&dec, &framePcm, &frameInfo, totalSamples - decodedSamples);
            if (frameSamples == 0) {
                break;
            }
            if (framePcm == nullptr || frameSamples % channels != 0) {
                return std::nullopt;
            }

            const auto frameCount = frameSamples / channels;
            for (std::size_t frame = 0; frame < frameCount; ++frame) {
                double channelSum = 0.0;
                for (std::size_t channel = 0; channel < channels; ++channel) {
                    channelSum += static_cast<double>(framePcm[frame * channels + channel]) *
                                  static_cast<double>(kInt16ToFloatNorm);
                }
                result.monoSamples.push_back(
                    static_cast<float>(channelSum / static_cast<double>(channels)));
            }
            decodedSamples += frameSamples;
        }

        if (dec.last_error != 0 || decodedSamples != totalSamples ||
            result.monoSamples.size() != totalFrames) {
            return std::nullopt;
        }

        return result;
    } catch (...) {
        return std::nullopt;
    }
}

} // namespace

std::optional<DecodedAudioData> Mp3Decoder::decodeFile(const std::string& filePath) noexcept {
    if (filePath.empty()) {
        return std::nullopt;
    }

    try {
        // Le chemin est fourni en UTF-8 (juce::File) ; sur Windows, les API de
        // fichiers étroites (fopen/CreateFileA) rejettent les chemins non-ASCII.
        // std::filesystem::path construit depuis un std::u8string convertit en
        // UTF-16 natif et permet d'ouvrir les chemins accentués.
        const auto utf8Path =
            std::filesystem::path(std::u8string(filePath.begin(), filePath.end()));

        std::error_code error;
        const auto fileSize = std::filesystem::file_size(utf8Path, error);
        if (error || fileSize <= 0 || static_cast<std::size_t>(fileSize) > kMaxMp3SizeBytes) {
            return std::nullopt;
        }

        mp3dec_ex_t dec;
#ifdef _WIN32
        if (mp3dec_ex_open_w(&dec, utf8Path.wstring().c_str(), MP3D_SEEK_TO_SAMPLE) != 0) {
            return std::nullopt;
        }
#else
        if (mp3dec_ex_open(&dec, filePath.c_str(), MP3D_SEEK_TO_SAMPLE) != 0) {
            return std::nullopt;
        }
#endif

        return processDecoder(dec);
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<DecodedAudioData>
Mp3Decoder::decodeMemory(const std::span<const std::uint8_t> data) noexcept {
    if (data.empty() || data.size() > kMaxMp3SizeBytes) {
        return std::nullopt;
    }

    try {
        mp3dec_ex_t dec{};
        if (mp3dec_ex_open_buf(&dec, data.data(), data.size(), MP3D_SEEK_TO_SAMPLE) != 0) {
            mp3dec_ex_close(&dec);
            return std::nullopt;
        }

        return processDecoder(dec);
    } catch (...) {
        return std::nullopt;
    }
}

} // namespace vocalmelody::audio
