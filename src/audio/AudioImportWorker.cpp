#include <vocalmelody/audio/AudioImportWorker.h>

#include <utility>

namespace vocalmelody::audio {

AudioImportWorker::~AudioImportWorker() { requestCancel(); }

void AudioImportWorker::start(std::string path, AudioImportProgressCallback progressCallback,
                              AudioImportCompletionCallback completionCallback) {
    requestCancel();
    if (thread_.joinable()) {
        thread_.join();
    }

    thread_ = std::jthread(
        [path = std::move(path), progress = std::move(progressCallback),
         completion = std::move(completionCallback)](const std::stop_token stopToken) mutable {
            auto outcome = AudioFileImporter{}.import(path, stopToken, std::move(progress));
            if (completion) {
                try {
                    completion(std::move(outcome));
                } catch (...) {
                    // Une notification cliente ne doit jamais terminer le processus.
                }
            }
        });
}

void AudioImportWorker::requestCancel() noexcept {
    if (thread_.joinable()) {
        thread_.request_stop();
    }
}

bool AudioImportWorker::joinable() const noexcept { return thread_.joinable(); }

} // namespace vocalmelody::audio
