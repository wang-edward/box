# For lots of helpful commentary on the commands used this file, look
# at the CMakeLists.txt it was based on:
# https://github.com/juce-framework/JUCE/blob/master/examples/CMake/ConsoleApp/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
project(BOX VERSION 0.0.1)

if (APPLE)
    add_definitions(-DGL_SILENCE_DEPRECATION)
endif()

add_subdirectory(external/tracktion_engine/modules/juce)
add_subdirectory(external/tracktion_engine/modules)
add_subdirectory(external/raylib)
add_subdirectory(external/googletest)
include_directories(include)
include_directories(external/googletest/googletest/include)

# Collect all .cc files from src directory
file(GLOB_RECURSE SRC_FILES src/*.cc)


add_library(BoxLib
    STATIC
    ${SRC_FILES}
)

target_link_libraries(BoxLib
    PRIVATE
        raylib
        tracktion::tracktion_engine
        juce::juce_core
        ${OPENGL_LIBRARIES}
        ${GLFW_LIBRARIES}
        ${GLEW_LIBRARIES}
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_link_libraries(Box PRIVATE "-latomic")
endif()


juce_add_console_app(Box
    PRODUCT_NAME "Box test")

target_sources(Box
    src/main.cc
)

target_compile_definitions(Box
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0)

target_link_libraries(Box 
    PRIVATE BoxLib)

file(COPY ${CMAKE_SOURCE_DIR}/shader DESTINATION ${CMAKE_BINARY_DIR}/Box_artefacts)

# test
add_executable(BoxTests
    test/test.cc
)

# Link the test executable with Google Test and the main application
target_link_libraries(BoxTests 
    PRIVATE 
        BoxLib
        gtest 
        gtest_main)
target_include_directories(BoxTests
    PRIVATE
        include
)
