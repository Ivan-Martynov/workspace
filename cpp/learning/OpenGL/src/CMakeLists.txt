include_directories("${PROJECT_SOURCE_DIR}/include")

# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(glad)
add_subdirectory(window_tools)
add_subdirectory(shader_tools)

add_subdirectory(01_starting)
