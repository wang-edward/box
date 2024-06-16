# For lots of helpful commentary on the commands used this file, look
# at the CMakeLists.txt it was based on:
# https://github.com/juce-framework/JUCE/blob/master/examples/CMake/ConsoleApp/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(BOX VERSION 0.0.1)

add_subdirectory(external/tracktion_engine/modules/juce)
add_subdirectory(external/tracktion_engine/modules)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

juce_add_console_app(Box
    PRODUCT_NAME "Box test")

target_sources(Box
    PRIVATE
        main.cc)

target_compile_definitions(Box
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0)

target_link_libraries(Box
    PRIVATE
        tracktion::tracktion_engine
        juce::juce_core
        ${CURSES_LIBRARIES}
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_link_libraries(Box PRIVATE "-latomic")
endif()
