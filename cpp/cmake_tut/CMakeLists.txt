cmake_minimum_required(VERSION 3.9.1 FATAL_ERROR)

set_target_properties(${TARGET} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

project(hello_world)

#target_include_directories(mylib PUBLIC include)
include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(main ${SOURCES})

