cmake_minimum_required(VERSION 3.10)

project(CommandDesignPattern VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Wpedantic -Wextra -Werror -Weffc++")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(TARGETS
    example1
    giant
    number
    person
)

foreach(TARGET ${TARGETS})
  add_executable(${TARGET}.exe src/${TARGET}.cpp)
endforeach(TARGET)
