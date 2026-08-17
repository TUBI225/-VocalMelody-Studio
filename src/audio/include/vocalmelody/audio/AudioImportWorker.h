#pragma once

#include <vocalmelody/audio/AudioFileImporter.h>

#include <functional>
#include <string>
#include <thread>

namespace vocalmelody::audio {

using AudioImportCompletionCallback = std::function<void(AudioImportOutcome)>;

// Exécute exactement un import hors du thread appelant. Un nouvel import annule
// et rejoint le précédent ; la destruction garantit également l'arrêt complet.
class AudioImportWorker final {
  public:
    AudioImportWorker() = default;
    ~AudioImportWorker();

    AudioImportWorker(const AudioImportWorker&) = delete;
    AudioImportWorker& operator=(const AudioImportWorker&) = delete;

    void start(std::string path, AudioImportProgressCallback progressCallback,
               AudioImportCompletionCallback completionCallback);
    void requestCancel() noexcept;
    [[nodiscard]] bool joinable() const noexcept;

  private:
    std::jthread thread_;
};

} // namespace vocalmelody::audio
