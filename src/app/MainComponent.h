#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace vocalmelody::app {

class MainComponent final : public juce::Component {
  public:
    MainComponent();

    void paint(juce::Graphics& graphics) override;
    void resized() override;

  private:
    juce::Label titleLabel_;
    juce::Label statusLabel_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

} // namespace vocalmelody::app
