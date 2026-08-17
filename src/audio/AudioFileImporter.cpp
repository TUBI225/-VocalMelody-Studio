#include <vocalmelody/audio/AudioFileImporter.h>

#include <vocalmelody/audio/Mp3Decoder.h>
#include <vocalmelody/frontend/SignalAnalysis.h>

#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_cryptography/juce_cryptography.h>

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <new>
#include <utility>
#include <vector>

namespace vocalmelody::audio {
namespace {

constexpr int kDecodeBlockFrames = 8192;

void reportProgress(const AudioImportProgressCallback& callback, const ImportStage stage,
                    const double fraction) noexcept {
    if (!callback) {
        return;
    }
    try {
        callback(AudioImportProgress{stage, std::clamp(fraction, 0.0, 1.0)});
    } catch (...) {
        // Le suivi est informatif : une UI défaillante ne doit pas interrompre l'import.
    }
}

class HashingInputStream final : public juce::InputStream {
  public:
    HashingInputStream(const juce::File& file, const std::stop_token stopToken,
                       const AudioImportProgressCallback& progressCallback, const ImportStage stage,
                       const double progressStart, const double progressEnd)
        : stream_(file), stopToken_(stopToken), progressCallback_(progressCallback), stage_(stage),
          progressStart_(progressStart), progressEnd_(progressEnd) {}

    [[nodiscard]] bool openedOk() const noexcept { return stream_.openedOk(); }
    [[nodiscard]] bool wasCancelled() const noexcept { return cancelled_; }

    int read(void* destinationBuffer, const int maxBytesToRead) override {
        if (stopToken_.stop_requested()) {
            cancelled_ = true;
            return 0;
        }
        const int bytesRead = stream_.read(destinationBuffer, maxBytesToRead);
        const auto length = getTotalLength();
        if (length > 0) {
            const double localFraction =
                static_cast<double>(getPosition()) / static_cast<double>(length);
            reportProgress(progressCallback_, stage_,
                           progressStart_ + (progressEnd_ - progressStart_) * localFraction);
        }
        return bytesRead;
    }

    [[nodiscard]] std::int64_t getPosition() override { return stream_.getPosition(); }
    bool setPosition(const std::int64_t newPosition) override {
        return stream_.setPosition(newPosition);
    }
    [[nodiscard]] std::int64_t getTotalLength() override { return stream_.getTotalLength(); }
    [[nodiscard]] bool isExhausted() override { return cancelled_ || stream_.isExhausted(); }

  private:
    juce::FileInputStream stream_;
    std::stop_token stopToken_;
    AudioImportProgressCallback progressCallback_;
    ImportStage stage_{ImportStage::HashingSource};
    double progressStart_{0.0};
    double progressEnd_{1.0};
    bool cancelled_{false};
};

struct RawDecodedData final {
    std::vector<float> mono;
    int sampleRate{0};
    int numChannels{0};
    int bitDepth{16};
    std::int64_t numFrames{0};
};

[[nodiscard]] std::optional<RawDecodedData>
decodeWithJuce(const juce::File& file, const std::stop_token stopToken,
               const AudioImportProgressCallback& progressCallback) {
    if (stopToken.stop_requested()) {
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
        reader->numChannels > static_cast<unsigned int>(AudioFileImporter::kMaxChannelCount) ||
        frameCount <= 0 || frameCount > AudioFileImporter::kMaxDecodedFrames ||
        static_cast<std::int64_t>(reader->numChannels) * frameCount >
            AudioFileImporter::kMaxDecodedSamples ||
        !std::isfinite(sourceSampleRate) || sourceSampleRate <= 0.0 ||
        sourceSampleRate > static_cast<double>(std::numeric_limits<int>::max()) ||
        frameCount > static_cast<std::int64_t>(std::numeric_limits<int>::max())) {
        return std::nullopt;
    }

    const int numChannels = static_cast<int>(reader->numChannels);
    const int numFrames = static_cast<int>(frameCount);
    const int sampleRate = static_cast<int>(std::lround(sourceSampleRate));
    RawDecodedData result{
        {}, sampleRate, numChannels, static_cast<int>(reader->bitsPerSample), frameCount};
    result.mono.reserve(static_cast<std::size_t>(numFrames));

    juce::AudioBuffer<float> buffer(numChannels, std::min(kDecodeBlockFrames, numFrames));
    for (int blockStart = 0; blockStart < numFrames; blockStart += kDecodeBlockFrames) {
        if (stopToken.stop_requested()) {
            return std::nullopt;
        }
        const int blockSize = std::min(kDecodeBlockFrames, numFrames - blockStart);
        if (!reader->read(&buffer, 0, blockSize, blockStart, true, true)) {
            return std::nullopt;
        }
        for (int frame = 0; frame < blockSize; ++frame) {
            double sum = 0.0;
            for (int channel = 0; channel < numChannels; ++channel) {
                sum += static_cast<double>(buffer.getSample(channel, frame));
            }
            result.mono.push_back(static_cast<float>(sum / static_cast<double>(numChannels)));
        }
        reportProgress(progressCallback, ImportStage::Decoding,
                       static_cast<double>(blockStart + blockSize) /
                           static_cast<double>(numFrames));
    }
    return result;
}

[[nodiscard]] std::optional<RawDecodedData>
decodeWithMp3(const std::string& path, const std::stop_token stopToken,
              const AudioImportProgressCallback& progressCallback) {
    const auto mp3Result =
        Mp3Decoder::decodeFile(path, stopToken, [&progressCallback](const double fraction) {
            reportProgress(progressCallback, ImportStage::Decoding, fraction);
        });
    if (!mp3Result.has_value()) {
        return std::nullopt;
    }

    return RawDecodedData{mp3Result->monoSamples, mp3Result->sampleRate, mp3Result->channelCount,
                          mp3Result->bitDepth, mp3Result->totalFrames};
}

[[nodiscard]] AudioImportOutcome cancelledOutcome() noexcept {
    return AudioImportOutcome::failure(ImportError::Cancelled);
}

} // namespace

AudioFileImporter::AudioFileImporter() = default;

std::string AudioFileImporter::computeFileHash(const std::string& path,
                                               const std::stop_token stopToken,
                                               const AudioImportProgressCallback& progress,
                                               const ImportStage stage, const double progressStart,
                                               const double progressEnd) {
    const juce::File file(path);
    if (!file.existsAsFile() || stopToken.stop_requested()) {
        return {};
    }
    HashingInputStream input(file, stopToken, progress, stage, progressStart, progressEnd);
    if (!input.openedOk()) {
        return {};
    }
    const auto hash = juce::SHA256(input).toHexString().toStdString();
    return input.wasCancelled() ? std::string{} : hash;
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
    case ImportError::Cancelled:
        return "Import annulé";
    case ImportError::InternalError:
        return "Erreur interne pendant l'import";
    }
    return "Erreur inconnue";
}

std::string_view importStageToString(const ImportStage stage) noexcept {
    switch (stage) {
    case ImportStage::Validating:
        return "Validation";
    case ImportStage::HashingSource:
        return "Empreinte du fichier";
    case ImportStage::Decoding:
        return "Décodage";
    case ImportStage::AnalyzingSource:
        return "Analyse de la source";
    case ImportStage::Resampling:
        return "Rééchantillonnage";
    case ImportStage::AnalyzingResult:
        return "Analyse du signal";
    case ImportStage::VerifyingSource:
        return "Vérification du fichier";
    case ImportStage::Finalizing:
        return "Finalisation";
    }
    return "Import";
}

AudioImportOutcome::AudioImportOutcome(AudioImportResult result) : state_(std::move(result)) {}

AudioImportOutcome::AudioImportOutcome(const ImportError error) noexcept : state_(error) {}

AudioImportOutcome AudioImportOutcome::success(AudioImportResult result) {
    return AudioImportOutcome(std::move(result));
}

AudioImportOutcome AudioImportOutcome::failure(const ImportError error) noexcept {
    return AudioImportOutcome(error);
}

bool AudioImportOutcome::has_value() const noexcept {
    return std::holds_alternative<AudioImportResult>(state_);
}

AudioImportOutcome::operator bool() const noexcept { return has_value(); }

const AudioImportResult& AudioImportOutcome::value() const {
    return std::get<AudioImportResult>(state_);
}

const AudioImportResult* AudioImportOutcome::operator->() const { return &value(); }

ImportError AudioImportOutcome::error() const { return std::get<ImportError>(state_); }

AudioImportOutcome
AudioFileImporter::import(const std::string& path, const std::stop_token stopToken,
                          AudioImportProgressCallback progressCallback) const noexcept {
    try {
        reportProgress(progressCallback, ImportStage::Validating, 0.0);
        if (stopToken.stop_requested()) {
            return cancelledOutcome();
        }
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

        const common::AudioFormat format =
            common::audioFormatFromExtension(file.getFileExtension().toStdString());
        if (format == common::AudioFormat::Unknown || format == common::AudioFormat::M4a) {
            return AudioImportOutcome::failure(ImportError::UnsupportedFormat);
        }
        reportProgress(progressCallback, ImportStage::Validating, 1.0);

        reportProgress(progressCallback, ImportStage::HashingSource, 0.0);
        const std::string fileHashBeforeDecode = computeFileHash(
            path, stopToken, progressCallback, ImportStage::HashingSource, 0.0, 1.0);
        if (stopToken.stop_requested()) {
            return cancelledOutcome();
        }
        if (fileHashBeforeDecode.empty()) {
            return AudioImportOutcome::failure(ImportError::FileNotFound);
        }

        reportProgress(progressCallback, ImportStage::Decoding, 0.0);
        std::optional<RawDecodedData> decodedData;
        if (format == common::AudioFormat::Mp3) {
            decodedData = decodeWithMp3(path, stopToken, progressCallback);
            if (!decodedData.has_value() && !stopToken.stop_requested()) {
                decodedData = decodeWithJuce(file, stopToken, progressCallback);
            }
        } else {
            decodedData = decodeWithJuce(file, stopToken, progressCallback);
        }
        if (stopToken.stop_requested()) {
            return cancelledOutcome();
        }
        if (!decodedData.has_value()) {
            return AudioImportOutcome::failure(ImportError::DecodeFailed);
        }

        const auto& mono = decodedData->mono;
        reportProgress(progressCallback, ImportStage::AnalyzingSource, 0.0);
        const auto sourceStats = frontend::analyzeSignal(mono, decodedData->sampleRate, stopToken);
        if (stopToken.stop_requested()) {
            return cancelledOutcome();
        }
        reportProgress(progressCallback, ImportStage::AnalyzingSource, 1.0);

        reportProgress(progressCallback, ImportStage::Resampling, 0.0);
        const auto analysisFrames = frontend::resampleWindowedSinc(
            mono, decodedData->sampleRate, kAnalysisSampleRate, stopToken,
            [&progressCallback](const double fraction) {
                reportProgress(progressCallback, ImportStage::Resampling, fraction);
            });
        if (stopToken.stop_requested()) {
            return cancelledOutcome();
        }
        if (!sourceStats.has_value() || !analysisFrames.has_value()) {
            return AudioImportOutcome::failure(ImportError::AnalysisFailed);
        }

        reportProgress(progressCallback, ImportStage::AnalyzingResult, 0.0);
        const auto analysisStats =
            frontend::analyzeSignal(*analysisFrames, kAnalysisSampleRate, stopToken);
        const auto silenceSegments =
            frontend::detectSilenceSegments(*analysisFrames, kAnalysisSampleRate, stopToken);
        const auto noiseFloor = frontend::estimateNoiseFloor(*analysisFrames, stopToken);
        if (stopToken.stop_requested()) {
            return cancelledOutcome();
        }
        if (!analysisStats.has_value()) {
            return AudioImportOutcome::failure(ImportError::AnalysisFailed);
        }
        reportProgress(progressCallback, ImportStage::AnalyzingResult, 1.0);

        const auto durationSeconds =
            common::Seconds::fromValue(static_cast<double>(decodedData->numFrames) /
                                       static_cast<double>(decodedData->sampleRate));
        const auto clippingScore = common::Score01::fromValue(sourceStats->clippingScore);
        const auto noiseScore = common::Score01::fromValue(
            noiseFloor.has_value() ? std::min(1.0, static_cast<double>(*noiseFloor) * 100.0) : 0.0);
        const auto voicePresenceScore =
            common::Score01::fromValue(1.0 - analysisStats->silenceRatio);
        const auto qualityScore = common::Score01::fromValue(
            std::clamp(1.0 - sourceStats->clippingScore * 2.0, 0.0, 1.0));
        if (!durationSeconds.has_value() || !clippingScore.has_value() || !noiseScore.has_value() ||
            !voicePresenceScore.has_value() || !qualityScore.has_value()) {
            return AudioImportOutcome::failure(ImportError::AnalysisFailed);
        }

        reportProgress(progressCallback, ImportStage::VerifyingSource, 0.0);
        const std::string fileHashAfterDecode = computeFileHash(
            path, stopToken, progressCallback, ImportStage::VerifyingSource, 0.0, 1.0);
        if (stopToken.stop_requested()) {
            return cancelledOutcome();
        }
        if (fileHashAfterDecode != fileHashBeforeDecode) {
            return AudioImportOutcome::failure(ImportError::FileModifiedDuringImport);
        }

        reportProgress(progressCallback, ImportStage::Finalizing, 0.0);
        const auto source = common::AudioSource::create(
            "audio-" + fileHashBeforeDecode, path,
            juce::Time::getCurrentTime().toISO8601(true).toStdString(), format,
            decodedData->sampleRate, decodedData->numChannels, decodedData->bitDepth,
            *durationSeconds, fileHashBeforeDecode);
        if (!source.has_value()) {
            return AudioImportOutcome::failure(ImportError::InvalidMetadata);
        }

        const auto analysis = common::AudioAnalysisResult::create(
            source->id(), kAnalysisVersion, *durationSeconds, kAnalysisSampleRate, {},
            *clippingScore, *noiseScore, *voicePresenceScore, silenceSegments, *qualityScore, {});
        if (!analysis.has_value()) {
            return AudioImportOutcome::failure(ImportError::InvalidMetadata);
        }
        reportProgress(progressCallback, ImportStage::Finalizing, 1.0);
        return AudioImportOutcome::success(AudioImportResult{*source, *analysis});
    } catch (const std::bad_alloc&) {
        return AudioImportOutcome::failure(ImportError::OutOfMemory);
    } catch (...) {
        return AudioImportOutcome::failure(ImportError::InternalError);
    }
}

} // namespace vocalmelody::audio
