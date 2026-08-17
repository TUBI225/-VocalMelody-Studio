#pragma once

#include <vocalmelody/common/AudioAnalysisResult.h>
#include <vocalmelody/common/AudioSource.h>

#include <cstdint>
#include <functional>
#include <optional>
#include <stop_token>
#include <string>
#include <string_view>
#include <variant>

namespace vocalmelody::audio {

struct AudioImportResult final {
    common::AudioSource source;
    common::AudioAnalysisResult analysis;
};

enum class ImportError {
    FileNotFound,
    EmptyFile,
    FileTooLarge,
    UnsupportedFormat,
    DecodeFailed,
    FileModifiedDuringImport,
    AnalysisFailed,
    InvalidMetadata,
    OutOfMemory,
    Cancelled,
    InternalError,
};

[[nodiscard]] std::string_view importErrorToString(const ImportError error) noexcept;

enum class ImportStage {
    Validating,
    HashingSource,
    Decoding,
    AnalyzingSource,
    Resampling,
    AnalyzingResult,
    VerifyingSource,
    Finalizing,
};

[[nodiscard]] std::string_view importStageToString(ImportStage stage) noexcept;

struct AudioImportProgress final {
    ImportStage stage{ImportStage::Validating};
    double fraction{0.0};
};

using AudioImportProgressCallback = std::function<void(const AudioImportProgress&)>;

// Résultat structuré de l'import : valeur en cas de succès, raison précise de
// l'échec sinon (remplace un `std::optional` muet pour distinguer les causes).
class AudioImportOutcome final {
  public:
    [[nodiscard]] static AudioImportOutcome success(AudioImportResult result);
    [[nodiscard]] static AudioImportOutcome failure(const ImportError error) noexcept;

    [[nodiscard]] bool has_value() const noexcept;
    [[nodiscard]] explicit operator bool() const noexcept;
    [[nodiscard]] const AudioImportResult& value() const;
    [[nodiscard]] const AudioImportResult* operator->() const;
    [[nodiscard]] ImportError error() const;

  private:
    explicit AudioImportOutcome(AudioImportResult result);
    explicit AudioImportOutcome(ImportError error) noexcept;

    std::variant<AudioImportResult, ImportError> state_;
};

class AudioFileImporter final {
  public:
    static constexpr std::int64_t kMaxFileSizeBytes = 1024LL * 1024LL * 1024LL;
    static constexpr std::int64_t kMaxDecodedFrames = 30'000'000LL;
    // Borne le buffer décodé (canaux x trames) avant toute allocation : un
    // en-tête forgé peut déclarer jusqu'à 65 535 canaux (champ uint16 du WAV).
    static constexpr int kMaxChannelCount = 64;
    static constexpr std::int64_t kMaxDecodedSamples = 100'000'000LL;
    static constexpr int kAnalysisSampleRate = 16'000;
    static constexpr int kAnalysisVersion = 4;

    AudioFileImporter();

    [[nodiscard]] AudioImportOutcome
    import(const std::string& path, std::stop_token stopToken = {},
           AudioImportProgressCallback progressCallback = {}) const noexcept;

  private:
    [[nodiscard]] static std::string computeFileHash(const std::string& path,
                                                     std::stop_token stopToken,
                                                     const AudioImportProgressCallback& progress,
                                                     ImportStage stage, double progressStart,
                                                     double progressEnd);
};

} // namespace vocalmelody::audio
