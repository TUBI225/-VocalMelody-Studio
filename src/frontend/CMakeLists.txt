add_library(vocalmelody_frontend STATIC
    SignalAnalysis.cpp
)
add_library(VocalMelody::Frontend ALIAS vocalmelody_frontend)

target_include_directories(
    vocalmelody_frontend
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(
    vocalmelody_frontend
    PRIVATE
        VocalMelody::Common
)

target_compile_features(vocalmelody_frontend PUBLIC cxx_std_20)

vocalmelody_enable_warnings(vocalmelody_frontend)
