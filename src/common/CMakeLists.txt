add_library(vocalmelody_common STATIC
    AudioSource.cpp
    AudioAnalysisResult.cpp
)
add_library(VocalMelody::Common ALIAS vocalmelody_common)

target_include_directories(
    vocalmelody_common
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_features(vocalmelody_common PUBLIC cxx_std_20)

vocalmelody_enable_warnings(vocalmelody_common)

