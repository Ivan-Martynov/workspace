cmake_minimum_required(VERSION 3.15)

project(OpenGLTutorial VERSION 1.1)

add_library(tutorial_compiler_flags INTERFACE)
target_compile_features(tutorial_compiler_flags INTERFACE cxx_std_17)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

target_compile_options(tutorial_compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-pedantic;-Werror;-Weffc++>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Configure a file by copying the input file and modyfing its content with the
# help of cmake's constants. 
configure_file(${PROJECT_SOURCE_DIR}/Config.h.in
               ${PROJECT_SOURCE_DIR}/include/Config.h)

add_subdirectory(src)
