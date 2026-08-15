juce_add_gui_app(
    VocalMelodyStudio
    PRODUCT_NAME "VocalMelody Studio"
    COMPANY_NAME "VocalMelody Studio"
    BUNDLE_ID "com.vocalmelody.studio"
)

target_sources(
    VocalMelodyStudio
    PRIVATE
        Main.cpp
        MainComponent.cpp
        MainComponent.h
)

target_compile_definitions(
    VocalMelodyStudio
    PRIVATE
        JUCE_USE_CURL=0
        JUCE_WEB_BROWSER=0
)

target_link_libraries(
    VocalMelodyStudio
    PRIVATE
        VocalMelody::Common
        VocalMelody::Audio
        juce::juce_gui_extra
        juce::juce_audio_devices
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
)

vocalmelody_enable_warnings(VocalMelodyStudio)
