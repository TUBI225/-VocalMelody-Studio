#include <vocalmelody/audio/AudioFileImporter.h>

#include <vocalmelody/frontend/SignalAnalysis.h>

#include <juce_audio_formats/juce_audio_formats.h>

#include <array>
#include <cstdio>
#include <vector>

namespace vocalmelody::audio {
namespace {

constexpr std::size_t kHashBufferSize = 4096;
constexpr uint64_t kFnvOffsetBasis = 14695981039346656037ULL;
constexpr uint64_t kFnvPrime = 1099511628211ULL;

[[nodiscard]] std::vector<float> readMonoSamples(const juce::AudioBuffer<float>& buffer) noexcept {
    const int numChannels = buffer.getNumChannels();
    const int numFrames = buffer.getNumSamples();

    if (numChannels >= 2) {
        std::vector<float> left(static_cast<std::size_t>(numFrames));
        std::vector<float> right(static_cast<std::size_t>(numFrames));
        for (int i = 0; i < numFrames; ++i) {
            left[static_cast<std::size_t>(i)] = buffer.getSample(0, i);
            right[static_cast<std::size_t>(i)] = buffer.getSample(1, i);
        }
        return frontend::downmixToMono(left, right);
    }

    std::vector<float> mono(static_cast<std::size_t>(numFrames));
    for (int i = 0; i < numFrames; ++i) {
        mono[static_cast<std::size_t>(i)] = buffer.getSample(0, i);
    }
    return mono;
}

} // namespace

AudioFileImporter::AudioFileImporter() = default;

std::string AudioFileImporter::computeFileHash(const std::string& path) noexcept {
    const juce::File file(path);
    juce::FileInputStream stream(file);
    if (stream.failedToOpen()) {
        return {};
    }

    uint64_t hash = kFnvOffsetBasis;
    std::array<char, kHashBufferSize> buffer{};
    while (true) {
        const auto read = stream.read(buffer.data(), static_cast<int>(kHashBufferSize));
        if (read <= 0) {
            break;
        }
        for (int i = 0; i < read; ++i) {
            hash ^= static_cast<uint64_t>(
                static_cast<unsigned char>(buffer[static_cast<std::size_t>(i)]));
            hash *= kFnvPrime;
        }
    }

    char hex[17]{};
    std::snprintf(hex, sizeof(hex), "%016llx", static_cast<unsigned long long>(hash));
    return std::string(hex);
}

std::optional<AudioImportResult> AudioFileImporter::import(const std::string& path) const noexcept {
    const juce::File file(path);
    if (!file.existsAsFile()) {
        return std::nullopt;
    }

    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
    if (reader == nullptr) {
        return std::nullopt;
    }

    const int numChannels = static_cast<int>(reader->numChannels);
    const int numFrames = static_cast<int>(reader->lengthInSamples);
    const int sampleRate = static_cast<int>(reader->sampleRate);
    if (numChannels <= 0 || numFrames <= 0 || sampleRate <= 0) {
        return std::nullopt;
    }

    juce::AudioBuffer<float> buffer(numChannels, numFrames);
    reader->read(&buffer, 0, numFrames, 0, true, true);

    const std::vector<float> mono = readMonoSamples(buffer);

    const auto stats = frontend::analyzeSignal(mono, sampleRate);
    if (!stats.has_value()) {
        return std::nullopt;
    }

    const auto silenceSegments = frontend::detectSilenceSegments(mono, sampleRate);
    const auto noiseFloor = frontend::estimateNoiseFloor(mono);

    const auto durationSeconds = common::Seconds::fromValue(static_cast<double>(numFrames) /
                                                            static_cast<double>(sampleRate));
    if (!durationSeconds.has_value()) {
        return std::nullopt;
    }

    const auto clippingScore = common::Score01::fromValue(stats->clippingScore);
    const auto noiseScore = common::Score01::fromValue(
        noiseFloor.has_value() ? std::min(1.0, static_cast<double>(*noiseFloor) * 100.0) : 0.0);
    const auto voicePresenceScore = common::Score01::fromValue(1.0 - stats->silenceRatio);
    const auto qualityScore =
        common::Score01::fromValue(std::min(1.0, std::max(0.0, 1.0 - stats->clippingScore * 2.0)));

    if (!clippingScore.has_value() || !noiseScore.has_value() || !voicePresenceScore.has_value() ||
        !qualityScore.has_value()) {
        return std::nullopt;
    }

    const std::string fileName = file.getFileName().toStdString();
    const common::AudioFormat format =
        common::audioFormatFromExtension(file.getFileExtension().toStdString());

    const auto source = common::AudioSource::create(
        fileName, path, "2026-08-14", format, sampleRate, numChannels,
        static_cast<int>(reader->bitsPerSample), *durationSeconds, computeFileHash(path));
    if (!source.has_value()) {
        return std::nullopt;
    }

    const auto analysis = common::AudioAnalysisResult::create(
        source->id(), 1, *durationSeconds, sampleRate, {}, *clippingScore, *noiseScore,
        *voicePresenceScore, silenceSegments, *qualityScore, {});
    if (!analysis.has_value()) {
        return std::nullopt;
    }

    return AudioImportResult{*source, *analysis};
}

} // namespace vocalmelody::audio
