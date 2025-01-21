cmake_minimum_required(VERSION 3.25.0)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(hello hello.cpp)
target_compile_features(hello PRIVATE cxx_lambda_init_captures)
