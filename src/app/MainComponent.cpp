#include "MainComponent.h"

#include <vocalmelody/audio/AudioFileImporter.h>
#include <vocalmelody/common/Version.h>

namespace vocalmelody::app {

MainComponent::MainComponent() {
    const auto title = vocalmelody::common::applicationName();
    titleLabel_.setText(juce::String::fromUTF8(title.data(), static_cast<int>(title.size())),
                        juce::dontSendNotification);
    titleLabel_.setFont(juce::Font{juce::FontOptions{28.0F, juce::Font::bold}});
    titleLabel_.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel_);

    statusLabel_.setText("Phase 1 - Audio Frontend", juce::dontSendNotification);
    statusLabel_.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(statusLabel_);

    diagnosticsLabel_.setText("Aucun fichier importé.", juce::dontSendNotification);
    diagnosticsLabel_.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(diagnosticsLabel_);

    importButton_.onClick = [this] { chooseFileToImport(); };
    addAndMakeVisible(importButton_);

    playButton_.onClick = [this] { togglePlayback(); };
    playButton_.setEnabled(false);
    addAndMakeVisible(playButton_);

    const auto deviceError = deviceManager_.initialiseWithDefaultDevices(0, 2);
    audioDeviceReady_ = deviceError.isEmpty();
    if (!audioDeviceReady_) {
        statusLabel_.setText("Périphérique audio indisponible : analyse seule.",
                             juce::dontSendNotification);
    }
    sourcePlayer_.setSource(&transportSource_);
    deviceManager_.addAudioCallback(&sourcePlayer_);

    setSize(900, 560);
}

MainComponent::~MainComponent() {
    deviceManager_.removeAudioCallback(&sourcePlayer_);
    sourcePlayer_.setSource(nullptr);
    transportSource_.setSource(nullptr);
    readerSource_.reset();
}

void MainComponent::paint(juce::Graphics& graphics) {
    graphics.fillAll(juce::Colour{0xFF10131A});
    graphics.setColour(juce::Colour{0xFF68D5C8});
    graphics.drawRoundedRectangle(getLocalBounds().toFloat().reduced(24.0F), 12.0F, 1.5F);
}

void MainComponent::resized() {
    auto content = getLocalBounds().reduced(48);
    titleLabel_.setBounds(content.removeFromTop(72));
    statusLabel_.setBounds(content.removeFromTop(40));
    diagnosticsLabel_.setBounds(content.removeFromTop(110));

    auto buttons = content.removeFromTop(40);
    importButton_.setBounds(buttons.removeFromLeft(140));
    playButton_.setBounds(buttons.removeFromLeft(140));
}

void MainComponent::chooseFileToImport() {
    fileChooser_ = std::make_unique<juce::FileChooser>(
        "Choisir un fichier audio", juce::File::getSpecialLocation(juce::File::userHomeDirectory),
        // M4A n'est pas encore pris en charge par l'import : seuls WAV et MP3 sont proposés.
        "*.wav;*.mp3");
    juce::Component::SafePointer<MainComponent> safeThis(this);
    fileChooser_->launchAsync(juce::FileBrowserComponent::openMode |
                                  juce::FileBrowserComponent::canSelectFiles,
                              [safeThis](const juce::FileChooser& chooser) {
                                  if (safeThis == nullptr) {
                                      return;
                                  }
                                  const auto file = chooser.getResult();
                                  if (file != juce::File()) {
                                      safeThis->loadFile(file);
                                  }
                              });
}

void MainComponent::loadFile(const juce::File& file) {
    const auto outcome =
        vocalmelody::audio::AudioFileImporter{}.import(file.getFullPathName().toStdString());
    if (!outcome.has_value()) {
        const auto reason = vocalmelody::audio::importErrorToString(outcome.error());
        statusLabel_.setText(
            "Import impossible : " +
                juce::String::fromUTF8(reason.data(), static_cast<int>(reason.size())),
            juce::dontSendNotification);
        return;
    }
    const auto& importResult = outcome.value();

    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
    if (reader == nullptr) {
        statusLabel_.setText("Lecture impossible.", juce::dontSendNotification);
        return;
    }

    transportSource_.stop();
    transportSource_.setSource(nullptr);
    readerSource_ = std::make_unique<juce::AudioFormatReaderSource>(reader.release(), true);
    transportSource_.setSource(
        readerSource_.get(), 0, nullptr, readerSource_->getAudioFormatReader()->sampleRate,
        static_cast<int>(readerSource_->getAudioFormatReader()->numChannels));
    playButton_.setButtonText("Lecture");
    playButton_.setEnabled(audioDeviceReady_);
    statusLabel_.setText(audioDeviceReady_ ? "Fichier chargé." : "Fichier analysé sans lecture.",
                         juce::dontSendNotification);

    const auto& source = importResult.source;
    const auto& analysis = importResult.analysis;

    juce::String diagnostics;
    const auto formatName = vocalmelody::common::audioFormatToString(source.originalFormat());
    diagnostics << "Fichier : " << juce::String(source.id()) << " | "
                << juce::String::fromUTF8(formatName.data(), static_cast<int>(formatName.size()))
                << " | " << source.sampleRate() << " Hz | " << source.channelCount() << " canaux"
                << " | " << juce::String(source.durationSeconds().value(), 3) << " s";
    diagnostics << "\nClipping : " << juce::String(analysis.clippingScore().value(), 3)
                << " | Silence : " << juce::String(1.0 - analysis.voicePresenceScore().value(), 3)
                << " | Qualité : " << juce::String(analysis.qualityScore().value(), 3)
                << " | Segments silence : " << static_cast<int>(analysis.silenceMap().size());
    diagnosticsLabel_.setText(diagnostics, juce::dontSendNotification);
}

void MainComponent::togglePlayback() {
    if (!audioDeviceReady_ || readerSource_ == nullptr) {
        return;
    }
    if (transportSource_.isPlaying()) {
        transportSource_.stop();
        playButton_.setButtonText("Lecture");
    } else {
        transportSource_.start();
        playButton_.setButtonText("Pause");
    }
}

} // namespace vocalmelody::app
