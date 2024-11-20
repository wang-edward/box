cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
project(BOX VERSION 0.0.1)

add_subdirectory(external/tracktion_engine/modules/juce)
add_subdirectory(external/tracktion_engine/modules)
add_subdirectory(external/raylib)
add_subdirectory(external/googletest)

# Specify CMake prefix path for Homebrew
set(CMAKE_PREFIX_PATH /opt/homebrew/opt/glfw /opt/homebrew/opt/glew)

# OpenGL
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(GLEW REQUIRED glew)
find_package(glm REQUIRED)

if (APPLE)
    add_definitions(-DGL_SILENCE_DEPRECATION)
endif()

# Includes
include_directories(include)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/external) # For stb_image
include_directories(/usr/include/GL)

# Collect all .cc files from src directory
file(GLOB_RECURSE SRC_FILES src/*.cc)

# Create a library for shared logic
add_library(BoxLib STATIC ${SRC_FILES})

target_compile_definitions(BoxLib
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0)

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
        juce::juce_recommended_warning_flags)

# Box executable
juce_add_console_app(Box
    PRODUCT_NAME "Box test")

target_sources(Box PRIVATE src/main.cc)

target_link_libraries(Box PRIVATE BoxLib)

file(COPY ${CMAKE_SOURCE_DIR}/shader DESTINATION ${CMAKE_BINARY_DIR}/Box_artefacts)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_link_libraries(Box PRIVATE "-latomic")
endif()

# Test executable
enable_testing()
add_executable(Test test/test.cpp)

target_link_libraries(Test
    PRIVATE
        GTest::gtest_main
        BoxLib
)

add_test(NAME ExampleTest COMMAND Test)
