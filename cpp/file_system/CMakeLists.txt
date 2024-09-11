cmake_minimum_required(VERSION 3.15)

project(FilesOperations VERSION 1.0)

add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_23)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

target_compile_options(compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-pedantic;-Werror;-Weffc++>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(TARGETS
    show_disk_usage
    show_files_in_directory
)

foreach(TARGET ${TARGETS})
  add_executable(${TARGET} src/${TARGET}.cpp)
  target_link_libraries(${TARGET} PUBLIC compiler_flags)
endforeach(TARGET)

add_executable(batch_rename_files
  src/batch_rename_files.cpp src/BatchFileRenamer.cpp src/FileNameValidator.cpp)
target_link_libraries(batch_rename_files PUBLIC compiler_flags)
