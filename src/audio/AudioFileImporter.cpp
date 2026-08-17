#include <vocalmelody/audio/AudioFileImporter.h>

#include <vocalmelody/audio/Mp3Decoder.h>
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

struct RawDecodedData final {
    std::vector<float> mono;
    int sampleRate{0};
    int numChannels{0};
    int bitDepth{16};
    std::int64_t numFrames{0};
};

[[nodiscard]] std::optional<RawDecodedData> decodeWithJuce(const juce::File& file) {
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
    if (reader == nullptr) {
        return std::nullopt;
    }

    const auto frameCount = reader->lengthInSamples;
    const double sourceSampleRate = reader->sampleRate;
    if (reader->numChannels == 0 ||
        reader->numChannels > static_cast<unsigned int>(AudioFileImporter::kMaxChannelCount) ||
        frameCount <= 0 || frameCount > AudioFileImporter::kMaxDecodedFrames ||
        static_cast<std::int64_t>(reader->numChannels) * static_cast<std::int64_t>(frameCount) >
            AudioFileImporter::kMaxDecodedSamples ||
        !std::isfinite(sourceSampleRate) || sourceSampleRate <= 0.0 ||
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

    return RawDecodedData{readMonoSamples(buffer), sampleRate, numChannels,
                          static_cast<int>(reader->bitsPerSample), frameCount};
}

[[nodiscard]] std::optional<RawDecodedData> decodeWithMp3(const std::string& path) {
    const auto mp3Result = Mp3Decoder::decodeFile(path);
    if (!mp3Result.has_value()) {
        return std::nullopt;
    }

    return RawDecodedData{mp3Result->monoSamples, mp3Result->sampleRate, mp3Result->channelCount,
                          mp3Result->bitDepth, mp3Result->totalFrames};
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

std::string_view importErrorToString(const ImportError error) noexcept {
    switch (error) {
    case ImportError::FileNotFound:
        return "Fichier introuvable";
    case ImportError::EmptyFile:
        return "Fichier vide";
    case ImportError::FileTooLarge:
        return "Fichier trop volumineux (1 Gio max)";
    case ImportError::UnsupportedFormat:
        return "Format non pris en charge";
    case ImportError::DecodeFailed:
        return "Décodage impossible ou fichier corrompu";
    case ImportError::FileModifiedDuringImport:
        return "Fichier modifié pendant l'import";
    case ImportError::AnalysisFailed:
        return "Analyse impossible";
    case ImportError::InvalidMetadata:
        return "Métadonnées invalides";
    case ImportError::OutOfMemory:
        return "Mémoire insuffisante";
    }
    return "Erreur inconnue";
}

AudioImportOutcome AudioImportOutcome::success(AudioImportResult result) noexcept {
    AudioImportOutcome outcome;
    outcome.result_ = std::move(result);
    return outcome;
}

AudioImportOutcome AudioImportOutcome::failure(const ImportError error) noexcept {
    AudioImportOutcome outcome;
    outcome.error_ = error;
    return outcome;
}

bool AudioImportOutcome::has_value() const noexcept { return result_.has_value(); }

AudioImportOutcome::operator bool() const noexcept { return result_.has_value(); }

const AudioImportResult& AudioImportOutcome::value() const { return *result_; }

const AudioImportResult* AudioImportOutcome::operator->() const { return &*result_; }

ImportError AudioImportOutcome::error() const noexcept { return error_; }

AudioImportOutcome AudioFileImporter::import(const std::string& path) const noexcept {
    try {
        const juce::File file(path);
        if (!file.existsAsFile()) {
            return AudioImportOutcome::failure(ImportError::FileNotFound);
        }
        if (file.getSize() <= 0) {
            return AudioImportOutcome::failure(ImportError::EmptyFile);
        }
        if (file.getSize() > kMaxFileSizeBytes) {
            return AudioImportOutcome::failure(ImportError::FileTooLarge);
        }
        const std::string fileHashBeforeDecode = computeFileHash(path);
        if (fileHashBeforeDecode.empty()) {
            return AudioImportOutcome::failure(ImportError::FileNotFound);
        }

        const common::AudioFormat format =
            common::audioFormatFromExtension(file.getFileExtension().toStdString());
        // Rejet avant décodage : un format inconnu (ou M4A, non implémenté) ne
        // doit pas être décodé intégralement puis jeté.
        if (format == common::AudioFormat::Unknown || format == common::AudioFormat::M4a) {
            return AudioImportOutcome::failure(ImportError::UnsupportedFormat);
        }

        std::optional<RawDecodedData> decodedData;
        if (format == common::AudioFormat::Mp3) {
            decodedData = decodeWithMp3(path);
            if (!decodedData.has_value()) {
                decodedData = decodeWithJuce(file);
            }
        } else {
            decodedData = decodeWithJuce(file);
        }
        if (!decodedData.has_value()) {
            return AudioImportOutcome::failure(ImportError::DecodeFailed);
        }

        const auto& mono = decodedData->mono;
        const int sampleRate = decodedData->sampleRate;
        const int numChannels = decodedData->numChannels;
        const int bitDepth = decodedData->bitDepth;
        const auto numFrames = decodedData->numFrames;

        const auto sourceStats = frontend::analyzeSignal(mono, sampleRate);
        const auto analysisFrames =
            frontend::resampleWindowedSinc(mono, sampleRate, kAnalysisSampleRate);
        if (!sourceStats.has_value() || !analysisFrames.has_value()) {
            return AudioImportOutcome::failure(ImportError::AnalysisFailed);
        }

        const auto analysisStats = frontend::analyzeSignal(*analysisFrames, kAnalysisSampleRate);
        if (!analysisStats.has_value()) {
            return AudioImportOutcome::failure(ImportError::AnalysisFailed);
        }

        const auto silenceSegments =
            frontend::detectSilenceSegments(*analysisFrames, kAnalysisSampleRate);
        const auto noiseFloor = frontend::estimateNoiseFloor(*analysisFrames);

        const auto durationSeconds = common::Seconds::fromValue(static_cast<double>(numFrames) /
                                                                static_cast<double>(sampleRate));
        if (!durationSeconds.has_value()) {
            return AudioImportOutcome::failure(ImportError::AnalysisFailed);
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
            return AudioImportOutcome::failure(ImportError::AnalysisFailed);
        }

        const std::string fileHashAfterDecode = computeFileHash(path);
        if (fileHashAfterDecode != fileHashBeforeDecode) {
            return AudioImportOutcome::failure(ImportError::FileModifiedDuringImport);
        }

        const auto source = common::AudioSource::create(
            "audio-" + fileHashBeforeDecode, path,
            juce::Time::getCurrentTime().toISO8601(true).toStdString(), format, sampleRate,
            numChannels, bitDepth, *durationSeconds, fileHashBeforeDecode);
        if (!source.has_value()) {
            return AudioImportOutcome::failure(ImportError::InvalidMetadata);
        }

        const auto analysis = common::AudioAnalysisResult::create(
            source->id(), kAnalysisVersion, *durationSeconds, kAnalysisSampleRate, {},
            *clippingScore, *noiseScore, *voicePresenceScore, silenceSegments, *qualityScore, {});
        if (!analysis.has_value()) {
            return AudioImportOutcome::failure(ImportError::InvalidMetadata);
        }

        return AudioImportOutcome::success(AudioImportResult{*source, *analysis});
    } catch (...) {
        return AudioImportOutcome::failure(ImportError::OutOfMemory);
    }
}

} // namespace vocalmelody::audio
