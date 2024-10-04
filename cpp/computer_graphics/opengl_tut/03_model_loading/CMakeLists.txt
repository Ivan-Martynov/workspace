cmake_minimum_required(VERSION 3.15)

project(01_Starting VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Wpedantic -Wextra -Werror -Weffc++")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(OpenGL)

set(COMMON_LIBS GLEW GL glfw SOIL assimp)

set(TARGETS
    model_loading
)

foreach(TARGET ${TARGETS})
    add_executable(${TARGET}.exe main.cpp)
    target_link_libraries(${TARGET}.exe ${COMMON_LIBS})
endforeach(TARGET)
