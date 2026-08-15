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

add_executable(VocalMelodyPitchTests unit/PitchStructuresTests.cpp)

target_link_libraries(VocalMelodyPitchTests PRIVATE VocalMelody::Common)
vocalmelody_enable_warnings(VocalMelodyPitchTests)

add_test(NAME pitch.structures COMMAND VocalMelodyPitchTests)

add_executable(VocalMelodyPitchEstimatorTests unit/AutocorrelationEstimatorTests.cpp)

target_link_libraries(VocalMelodyPitchEstimatorTests PRIVATE VocalMelody::Pitch VocalMelody::Common)
vocalmelody_enable_warnings(VocalMelodyPitchEstimatorTests)

add_test(NAME pitch.autocorrelation COMMAND VocalMelodyPitchEstimatorTests)

if(TARGET VocalMelody::Audio)
    add_executable(VocalMelodyAudioImporterTests unit/AudioImporterTests.cpp)

    target_link_libraries(VocalMelodyAudioImporterTests PRIVATE VocalMelody::Audio)
    target_compile_definitions(
        VocalMelodyAudioImporterTests
        PRIVATE VOCALMELODY_MP3_TEST_VECTOR="${minimp3_SOURCE_DIR}/vectors/l3-sin1k0db.bit"
    )
    vocalmelody_enable_warnings(VocalMelodyAudioImporterTests)

    add_test(NAME audio.file_import COMMAND VocalMelodyAudioImporterTests)

    add_executable(VocalMelodyMp3Tests unit/Mp3DecoderTests.cpp)

    target_link_libraries(VocalMelodyMp3Tests PRIVATE VocalMelody::Audio)
    target_compile_definitions(
        VocalMelodyMp3Tests
        PRIVATE VOCALMELODY_MP3_TEST_VECTOR="${minimp3_SOURCE_DIR}/vectors/l3-sin1k0db.bit"
    )
    vocalmelody_enable_warnings(VocalMelodyMp3Tests)

    add_test(NAME audio.mp3_decoder COMMAND VocalMelodyMp3Tests)

    add_executable(VocalMelodyAudioMetadataTests unit/AudioMetadataTests.cpp)

    target_link_libraries(VocalMelodyAudioMetadataTests PRIVATE VocalMelody::Audio)
    vocalmelody_enable_warnings(VocalMelodyAudioMetadataTests)

    add_test(NAME audio.metadata COMMAND VocalMelodyAudioMetadataTests)
endif()
