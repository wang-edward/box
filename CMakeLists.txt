cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(GRAPHICS "GRAPHICS_API_OPENGL_21" CACHE STRING "Raylib graphics mode" FORCE) # use opengl2.1 for cm4
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
    # atomic
    target_link_libraries(BoxLib PUBLIC "-latomic")
    # gtk3
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    include_directories(${GTK3_INCLUDE_DIRS})
    link_directories(${GTK3_LIBRARY_DIRS})
    add_definitions(${GTK3_CFLAGS_OTHER})

    # webkit
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(WEBKIT2 REQUIRED webkit2gtk-4.0)
    include_directories(${WEBKIT2_INCLUDE_DIRS})
    link_directories(${WEBKIT2_LIBRARY_DIRS})
    add_definitions(${WEBKIT2_CFLAGS_OTHER})

    # curl
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBCURL REQUIRED libcurl)
    include_directories(${LIBCURL_INCLUDE_DIRS})
    link_directories(${LIBCURL_LIBRARY_DIRS})
    add_definitions(${LIBCURL_CFLAGS_OTHER})
    target_link_libraries(BoxLib PRIVATE ${LIBCURL_LIBRARIES})
endif()

# box
juce_add_console_app(Box
    PRODUCT_NAME "Box"
    NEEDS_WEB_BROWSER FALSE
    )
target_sources(Box
    PRIVATE
        src/main.cc)
target_compile_definitions(Box
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0)
target_link_libraries(Box
    PRIVATE
        BoxLib)

# boxtest
enable_testing()
file(GLOB_RECURSE TEST_SRC_FILES test/unit_test.cc test/test_util.cc)
add_executable(UnitTests
    ${TEST_SRC_FILES})
target_include_directories(UnitTests
    PUBLIC
        test)
target_link_libraries(UnitTests
    PRIVATE 
        BoxLib
        gtest 
        gtest_main)

# fuzz
file(GLOB_RECURSE FUZZ_SRC_FILES test/fuzz.cc test/test_util.cc)
juce_add_console_app(Fuzz
    PRODUCT_NAME "Fuzz"
    NEEDS_WEB_BROWSER FALSE
    )
target_sources(Fuzz
    PRIVATE
        ${FUZZ_SRC_FILES})
target_include_directories(Fuzz
    PUBLIC
        test)
target_link_libraries(Fuzz
    PRIVATE
        BoxLib)

# fake_main
juce_add_console_app(FakeMain
    PRODUCT_NAME "FakeMain"
    NEEDS_WEB_BROWSER FALSE
    )
target_sources(FakeMain
    PRIVATE
        test/fake_main.cc)
target_compile_definitions(FakeMain
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0)
target_link_libraries(FakeMain
    PRIVATE
        BoxLib
        gtest
        gtest_main)

# Register tests with ctest
# add_test(NAME BoxTests COMMAND BoxTests)
# set_tests_properties(BoxTests PROPERTIES CONFIGURATIONS Debug Release)
