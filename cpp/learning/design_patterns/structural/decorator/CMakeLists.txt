cmake_minimum_required(VERSION 3.15)

project(DecoratorPattern VERSION 1.0)

add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_23)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

target_compile_options(compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-pedantic;-Werror;-Weffc++>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(TARGETS
    concept
    string_wrapper
    text_widget
    xyz
)

foreach(TARGET ${TARGETS})
  add_executable(${TARGET} src/${TARGET}.cpp)
  target_link_libraries(${TARGET} PUBLIC compiler_flags)
endforeach(TARGET)

