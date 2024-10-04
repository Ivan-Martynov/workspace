cmake_minimum_required(VERSION 3.9.1)

set_target_properties(${TARGET} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

project(Tutorial)

set(Tutorial_VERSION_MAJOR 1)
set(Tutorial_VERSION_MINOR 0)
#LIBS = -lGL -lGLU -lGLEW -lGLEW -lglfw -lXxf86vm -ldl -lX11 -lpthread\
#	-lXrandr -lXi -lXinerama -lXcursor

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -pedantic -Werror -Weffc++")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(OpenGL)

set(COMMON_LIBS glfw dl openal audio stdc++fs)
set(COMMON_LIBS ${COMMON_LIBS} ${EXTRA_LIBS})

set(EXAMPLES
    01_simplePlot2D
    02_simplePingPong
    03_PingPong_with_sound
    )

foreach(EXAMPLE ${EXAMPLES})
  add_executable(${EXAMPLE} src/${EXAMPLE}/${EXAMPLE}.cpp src/glad.c)
  #set_property(TARGET ${EXAMPLE} PROPERTY DEBUG_POSTFIX _d)
  #  target_compile_options(${EXAMPLE} PRIVATE "-Wall -Wextra -Wshadow -pedantic -Werror")
  target_link_libraries(${EXAMPLE} ${COMMON_LIBS})
endforeach(EXAMPLE)

include_directories(include)

#file(GLOB SOURCES "src/*.cpp" "src/*.c*")

#add_executable(main ${SOURCES})

#target_link_libraries(main ${COMMON_LIBS})

