add_library(vocalmelody_audio STATIC
    AudioFileImporter.cpp
)
add_library(VocalMelody::Audio ALIAS vocalmelody_audio)

target_include_directories(
    vocalmelody_audio
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(
    vocalmelody_audio
    PUBLIC
        VocalMelody::Common
        VocalMelody::Frontend
        juce::juce_audio_formats
        juce::juce_audio_basics
)

target_compile_features(vocalmelody_audio PUBLIC cxx_std_20)

vocalmelody_enable_warnings(vocalmelody_audio)
