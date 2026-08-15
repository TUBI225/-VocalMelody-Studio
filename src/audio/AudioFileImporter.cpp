#include <vocalmelody/audio/AudioFileImporter.h>

#include <vocalmelody/frontend/SignalAnalysis.h>

#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_cryptography/juce_cryptography.h>

#include <cmath>
#include <limits>
#include <vector>

namespace vocalmelody::audio {
namespace {

[[nodiscard]] std::vector<float> readMonoSamples(const juce::AudioBuffer<float>& buffer) {
    const int numChannels = buffer.getNumChannels();
    const int numFrames = buffer.getNumSamples();

    std::vector<float> mono(static_cast<std::size_t>(numFrames));
    for (int frame = 0; frame < numFrames; ++frame) {
        double sum = 0.0;
        for (int channel = 0; channel < numChannels; ++channel) {
            sum += static_cast<double>(buffer.getSample(channel, frame));
        }
        mono[static_cast<std::size_t>(frame)] =
            static_cast<float>(sum / static_cast<double>(numChannels));
    }
    return mono;
}

} // namespace

AudioFileImporter::AudioFileImporter() = default;

std::string AudioFileImporter::computeFileHash(const std::string& path) {
    const juce::File file(path);
    if (!file.existsAsFile()) {
        return {};
    }
    return juce::SHA256(file).toHexString().toStdString();
}

std::optional<AudioImportResult> AudioFileImporter::import(const std::string& path) const noexcept {
    try {
        const juce::File file(path);
        if (!file.existsAsFile() || file.getSize() <= 0 || file.getSize() > kMaxFileSizeBytes) {
            return std::nullopt;
        }
        const std::string fileHashBeforeDecode = computeFileHash(path);
        if (fileHashBeforeDecode.empty()) {
            return std::nullopt;
        }

        juce::AudioFormatManager formatManager;
        formatManager.registerBasicFormats();
        std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
        if (reader == nullptr) {
            return std::nullopt;
        }

        const auto frameCount = reader->lengthInSamples;
        const double sourceSampleRate = reader->sampleRate;
        if (reader->numChannels == 0 ||
            reader->numChannels > static_cast<unsigned int>(std::numeric_limits<int>::max()) ||
            frameCount <= 0 || frameCount > kMaxDecodedFrames || !std::isfinite(sourceSampleRate) ||
            sourceSampleRate <= 0.0 ||
            sourceSampleRate > static_cast<double>(std::numeric_limits<int>::max())) {
            return std::nullopt;
        }

        const int numChannels = static_cast<int>(reader->numChannels);
        const int numFrames = static_cast<int>(frameCount);
        const int sampleRate = static_cast<int>(std::lround(sourceSampleRate));

        juce::AudioBuffer<float> buffer(numChannels, numFrames);
        if (!reader->read(&buffer, 0, numFrames, 0, true, true)) {
            return std::nullopt;
        }

        const std::vector<float> mono = readMonoSamples(buffer);

        const auto sourceStats = frontend::analyzeSignal(mono, sampleRate);
        const auto analysisFrames = frontend::resampleLinear(mono, sampleRate, kAnalysisSampleRate);
        if (!sourceStats.has_value() || !analysisFrames.has_value()) {
            return std::nullopt;
        }

        const auto analysisStats = frontend::analyzeSignal(*analysisFrames, kAnalysisSampleRate);
        if (!analysisStats.has_value()) {
            return std::nullopt;
        }

        const auto silenceSegments =
            frontend::detectSilenceSegments(*analysisFrames, kAnalysisSampleRate);
        const auto noiseFloor = frontend::estimateNoiseFloor(*analysisFrames);

        const auto durationSeconds = common::Seconds::fromValue(static_cast<double>(numFrames) /
                                                                static_cast<double>(sampleRate));
        if (!durationSeconds.has_value()) {
            return std::nullopt;
        }

        const auto clippingScore = common::Score01::fromValue(sourceStats->clippingScore);
        const auto noiseScore = common::Score01::fromValue(
            noiseFloor.has_value() ? std::min(1.0, static_cast<double>(*noiseFloor) * 100.0) : 0.0);
        const auto voicePresenceScore =
            common::Score01::fromValue(1.0 - analysisStats->silenceRatio);
        const auto qualityScore = common::Score01::fromValue(
            std::min(1.0, std::max(0.0, 1.0 - sourceStats->clippingScore * 2.0)));

        if (!clippingScore.has_value() || !noiseScore.has_value() ||
            !voicePresenceScore.has_value() || !qualityScore.has_value()) {
            return std::nullopt;
        }

        const std::string fileHashAfterDecode = computeFileHash(path);
        if (fileHashAfterDecode != fileHashBeforeDecode) {
            return std::nullopt;
        }
        const common::AudioFormat format =
            common::audioFormatFromExtension(file.getFileExtension().toStdString());

        const auto source = common::AudioSource::create(
            "audio-" + fileHashBeforeDecode, path,
            juce::Time::getCurrentTime().toISO8601(true).toStdString(), format, sampleRate,
            numChannels, static_cast<int>(reader->bitsPerSample), *durationSeconds,
            fileHashBeforeDecode);
        if (!source.has_value()) {
            return std::nullopt;
        }

        const auto analysis = common::AudioAnalysisResult::create(
            source->id(), 2, *durationSeconds, kAnalysisSampleRate, {}, *clippingScore, *noiseScore,
            *voicePresenceScore, silenceSegments, *qualityScore, {});
        if (!analysis.has_value()) {
            return std::nullopt;
        }

        return AudioImportResult{*source, *analysis};
    } catch (...) {
        return std::nullopt;
    }
}

} // namespace vocalmelody::audio
