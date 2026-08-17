#pragma once

#include <vocalmelody/audio/AudioImportWorker.h>

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include <memory>

namespace vocalmelody::app {

class MainComponent final : public juce::Component {
  public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics& graphics) override;
    void resized() override;

  private:
    void chooseFileToImport();
    void startImport(const juce::File& file);
    void finishImport(const juce::File& file, vocalmelody::audio::AudioImportOutcome outcome);
    void updateImportProgress(const vocalmelody::audio::AudioImportProgress& progress);
    void cancelImport();
    void loadForPlayback(const juce::File& file);
    void togglePlayback();

    juce::Label titleLabel_;
    juce::Label statusLabel_;
    juce::Label diagnosticsLabel_;
    juce::TextButton importButton_{"Importer..."};
    juce::TextButton cancelButton_{"Annuler"};
    juce::TextButton playButton_{"Lecture"};

    juce::AudioDeviceManager deviceManager_;
    juce::AudioFormatManager formatManager_;
    juce::AudioSourcePlayer sourcePlayer_;
    juce::AudioTransportSource transportSource_;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource_;
    std::unique_ptr<juce::FileChooser> fileChooser_;
    vocalmelody::audio::AudioImportWorker importWorker_;
    bool audioDeviceReady_{false};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

} // namespace vocalmelody::app
