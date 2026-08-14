#include "MainComponent.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <vocalmelody/common/Version.h>

#include <memory>

namespace vocalmelody::app {

class VocalMelodyApplication final : public juce::JUCEApplication {
  public:
    [[nodiscard]] const juce::String getApplicationName() override {
        const auto name = vocalmelody::common::applicationName();
        return juce::String::fromUTF8(name.data(), static_cast<int>(name.size()));
    }

    [[nodiscard]] const juce::String getApplicationVersion() override {
        const auto version = vocalmelody::common::applicationVersion();
        return juce::String::fromUTF8(version.data(), static_cast<int>(version.size()));
    }

    [[nodiscard]] bool moreThanOneInstanceAllowed() override { return false; }

    void initialise(const juce::String&) override {
        mainWindow_ = std::make_unique<MainWindow>(getApplicationName());
    }

    void shutdown() override { mainWindow_.reset(); }

    void systemRequestedQuit() override { quit(); }

    void anotherInstanceStarted(const juce::String&) override {
        if (mainWindow_ != nullptr) {
            mainWindow_->toFront(true);
        }
    }

  private:
    class MainWindow final : public juce::DocumentWindow {
      public:
        explicit MainWindow(const juce::String& name)
            : DocumentWindow{name, juce::Colour{0xFF10131A}, DocumentWindow::allButtons} {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent{}, true);
            centreWithSize(getWidth(), getHeight());
            setVisible(true);
        }

        void closeButtonPressed() override {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }

      private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

    std::unique_ptr<MainWindow> mainWindow_;
};

} // namespace vocalmelody::app

START_JUCE_APPLICATION(vocalmelody::app::VocalMelodyApplication)
