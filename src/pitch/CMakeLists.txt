add_library(vocalmelody_pitch STATIC
    AutocorrelationPitchEstimator.cpp
)
add_library(VocalMelody::Pitch ALIAS vocalmelody_pitch)

target_include_directories(
    vocalmelody_pitch
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(
    vocalmelody_pitch
    PRIVATE
        VocalMelody::Common
)

target_compile_features(vocalmelody_pitch PUBLIC cxx_std_20)

vocalmelody_enable_warnings(vocalmelody_pitch)
