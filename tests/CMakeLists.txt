add_executable(VocalMelodyCommonTests unit/StrongTypesTests.cpp)

target_link_libraries(VocalMelodyCommonTests PRIVATE VocalMelody::Common)
vocalmelody_enable_warnings(VocalMelodyCommonTests)

add_test(NAME common.strong_types COMMAND VocalMelodyCommonTests)

add_executable(VocalMelodyJsonTests unit/JsonWriterTests.cpp)

target_link_libraries(VocalMelodyJsonTests PRIVATE VocalMelody::Common)
vocalmelody_enable_warnings(VocalMelodyJsonTests)

add_test(NAME common.json_writer COMMAND VocalMelodyJsonTests)

add_executable(VocalMelodyFrontendTests unit/SignalAnalysisTests.cpp)

target_link_libraries(VocalMelodyFrontendTests PRIVATE VocalMelody::Frontend VocalMelody::Common)
vocalmelody_enable_warnings(VocalMelodyFrontendTests)

add_test(NAME frontend.signal_analysis COMMAND VocalMelodyFrontendTests)

if(TARGET VocalMelody::Audio)
    add_executable(VocalMelodyAudioImporterTests unit/AudioImporterTests.cpp)

    target_link_libraries(VocalMelodyAudioImporterTests PRIVATE VocalMelody::Audio)
    vocalmelody_enable_warnings(VocalMelodyAudioImporterTests)

    add_test(NAME audio.file_import COMMAND VocalMelodyAudioImporterTests)

    add_executable(VocalMelodyAudioMetadataTests unit/AudioMetadataTests.cpp)

    target_link_libraries(VocalMelodyAudioMetadataTests PRIVATE VocalMelody::Audio)
    vocalmelody_enable_warnings(VocalMelodyAudioMetadataTests)

    add_test(NAME audio.metadata COMMAND VocalMelodyAudioMetadataTests)
endif()
