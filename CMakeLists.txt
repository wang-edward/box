# For lots of helpful commentary on the commands used this file, look
# at the CMakeLists.txt it was based on:
# https://github.com/juce-framework/JUCE/blob/master/examples/CMake/ConsoleApp/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
project(BOX VERSION 0.0.1)

add_subdirectory(external/tracktion_engine/modules/juce)
add_subdirectory(external/tracktion_engine/modules)

# Specify CMake prefix path for Homebrew
set(CMAKE_PREFIX_PATH /opt/homebrew/opt/glfw /opt/homebrew/opt/glew)

# opengl
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(GLEW REQUIRED glew)

if (APPLE)
    add_definitions(-DGL_SILENCE_DEPRECATION)
endif()

# includes
include_directories(include)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

# Collect all .cc files from src directory
file(GLOB SRC_FILES src/*.cc)

juce_add_console_app(Box
    PRODUCT_NAME "Box test")

target_sources(Box
    PRIVATE
        ${SRC_FILES})

target_compile_definitions(Box
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0)

target_link_libraries(Box
    PRIVATE
        tracktion::tracktion_engine
        juce::juce_core
        ${OPENGL_LIBRARIES}
        ${GLFW_LIBRARIES}
        ${GLEW_LIBRARIES}
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_link_libraries(Box PRIVATE "-latomic")
endif()
