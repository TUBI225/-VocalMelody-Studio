cmake_minimum_required(VERSION 3.24)

project(
    VocalMelodyStudio
    VERSION 0.1.0
    DESCRIPTION "Vocal intent reconstruction and assisted music composition"
    LANGUAGES C CXX
)

include(CTest)
include(FetchContent)

option(VOCALMELODY_BUILD_APP "Build the JUCE desktop application" ON)
option(VOCALMELODY_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(VocalMelodyWarnings)

add_subdirectory(src/common)
add_subdirectory(src/frontend)

if(VOCALMELODY_BUILD_APP)
    FetchContent_Declare(
        JUCE
        URL https://github.com/juce-framework/JUCE/archive/91ad83ae34a81e0833b1a2b0866f54846370ae53.tar.gz
        URL_HASH SHA256=04f8d5055382582c757be9da069ea98338005f98248facd9c2804435ac853e70
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
    )
    FetchContent_MakeAvailable(JUCE)
    add_subdirectory(src/app)
    add_subdirectory(src/audio)
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
