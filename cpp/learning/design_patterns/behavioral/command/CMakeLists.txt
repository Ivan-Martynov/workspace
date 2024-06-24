cmake_minimum_required(VERSION 3.10)

project(CommandDesignPattern VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_subdirectory(src)

target_include_directories(CommandDesignPattern PUBLIC
    "${PROJECT_SOURCE_DIR}$/src")

