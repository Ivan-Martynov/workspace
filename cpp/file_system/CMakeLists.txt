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
  src/batch_rename_files.cpp
  src/StringHelper.cpp
  src/BatchFileRenamer.cpp
  src/FileRenameCommandBase.cpp
  src/CaseModifyCommand.cpp
  src/StringReplaceCommand.cpp
  src/FileOverwritePrompt.cpp
  src/StringAppendCommand.cpp
  src/Utilities.cpp
  src/FileNameValidator.cpp)

add_executable(organize_files
  src/organize_files.cpp
  src/FileOrganizer.cpp
  src/StringHelper.cpp
  src/Utilities.cpp
)

add_executable(mp3_batch_rename_files
  src/mp3_batch_rename_files.cpp
  src/MP3BatchFileRenamer.cpp
  src/BatchFileRenamer.cpp
  src/FileOverwritePrompt.cpp
  src/FileRenameCommandBase.cpp
  src/CaseModifyCommand.cpp
  src/StringReplaceCommand.cpp
  src/StringAppendCommand.cpp
  src/FileNameValidator.cpp
  src/MP3Tag.cpp
  src/StringHelper.cpp
  src/SyncsafeIntConverter.cpp
  src/Utilities.cpp
)

target_link_libraries(batch_rename_files PUBLIC compiler_flags)
target_link_libraries(organize_files PUBLIC compiler_flags)
target_link_libraries(mp3_batch_rename_files  PUBLIC compiler_flags)
