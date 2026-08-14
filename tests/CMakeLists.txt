add_executable(VocalMelodyCommonTests unit/StrongTypesTests.cpp)

target_link_libraries(VocalMelodyCommonTests PRIVATE VocalMelody::Common)
vocalmelody_enable_warnings(VocalMelodyCommonTests)

add_test(NAME common.strong_types COMMAND VocalMelodyCommonTests)

add_executable(VocalMelodyFrontendTests unit/SignalAnalysisTests.cpp)

target_link_libraries(VocalMelodyFrontendTests PRIVATE VocalMelody::Frontend VocalMelody::Common)
vocalmelody_enable_warnings(VocalMelodyFrontendTests)

add_test(NAME frontend.signal_analysis COMMAND VocalMelodyFrontendTests)

if(TARGET VocalMelody::Audio)
    add_executable(VocalMelodyAudioImporterTests unit/AudioImporterTests.cpp)

    target_link_libraries(VocalMelodyAudioImporterTests PRIVATE VocalMelody::Audio)
    vocalmelody_enable_warnings(VocalMelodyAudioImporterTests)

    add_test(NAME audio.file_import COMMAND VocalMelodyAudioImporterTests)
endif()
