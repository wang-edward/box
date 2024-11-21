cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
project(BOX VERSION 0.0.1)

if (APPLE)
    add_definitions(-DGL_SILENCE_DEPRECATION)
endif()

# shader
file(COPY ${CMAKE_SOURCE_DIR}/shader DESTINATION ${CMAKE_BINARY_DIR}/Box_artefacts)

add_subdirectory(external/tracktion_engine/modules/juce)
add_subdirectory(external/tracktion_engine/modules)
add_subdirectory(external/raylib)
add_subdirectory(external/googletest)
include_directories(include)
include_directories(external/googletest/googletest/include)

# sources (without main.cc)
file(GLOB_RECURSE SRC_FILES src/*.cc)
list(REMOVE_ITEM SRC_FILES ${CMAKE_SOURCE_DIR}/src/main.cc)

# boxlib
add_library(BoxLib
    STATIC
    ${SRC_FILES})
target_include_directories(BoxLib 
    PUBLIC
    external/tracktion_engine/modules
    external/tracktion_engine/modules/juce)
target_link_libraries(BoxLib
    PUBLIC
        raylib
        tracktion::tracktion_engine
        juce::juce_core
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(BoxLib PUBLIC "-latomic")
endif()

# box
juce_add_console_app(Box
    PRODUCT_NAME "Box")
target_sources(Box
    PRIVATE
        src/main.cc)
target_compile_definitions(Box PRIVATE JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0)
target_link_libraries(Box
    PRIVATE
        BoxLib)

# boxtest
enable_testing()
add_executable(BoxTests
    test/test.cc)
target_link_libraries(BoxTests 
    PRIVATE 
        BoxLib
        gtest 
        gtest_main)

# Register tests with ctest
add_test(NAME BoxTests COMMAND BoxTests)
# set_tests_properties(BoxTests PROPERTIES CONFIGURATIONS Debug Release)
