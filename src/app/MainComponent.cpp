#include "MainComponent.h"

#include <vocalmelody/common/Version.h>

namespace vocalmelody::app {

MainComponent::MainComponent() {
    const auto title = vocalmelody::common::applicationName();
    titleLabel_.setText(juce::String::fromUTF8(title.data(), static_cast<int>(title.size())),
                        juce::dontSendNotification);
    titleLabel_.setFont(juce::Font{juce::FontOptions{28.0F, juce::Font::bold}});
    titleLabel_.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel_);

    statusLabel_.setText("Phase 0 - socle technique", juce::dontSendNotification);
    statusLabel_.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(statusLabel_);

    setSize(900, 560);
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
}

} // namespace vocalmelody::app
