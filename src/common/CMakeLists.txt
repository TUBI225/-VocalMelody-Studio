add_library(vocalmelody_common INTERFACE)
add_library(VocalMelody::Common ALIAS vocalmelody_common)

target_include_directories(
    vocalmelody_common
    INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_features(vocalmelody_common INTERFACE cxx_std_20)
