#pragma once

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
    void loadFile(const juce::File& file);
    void togglePlayback();

    juce::Label titleLabel_;
    juce::Label statusLabel_;
    juce::Label diagnosticsLabel_;
    juce::TextButton importButton_{"Importer..."};
    juce::TextButton playButton_{"Lecture"};

    juce::AudioDeviceManager deviceManager_;
    juce::AudioSourcePlayer sourcePlayer_;
    juce::AudioTransportSource transportSource_;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource_;
    std::unique_ptr<juce::FileChooser> fileChooser_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

} // namespace vocalmelody::app
