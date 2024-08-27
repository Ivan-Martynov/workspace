set(TARGETS
    hello_window
    hello_triangle
    hello_shaders
    hello_texture
)

foreach(EXAMPLE ${TARGETS})
    add_executable(${EXAMPLE} ${EXAMPLE}.cpp)
    target_link_libraries(${EXAMPLE} PUBLIC tutorial_compiler_flags glad glfw)
endforeach(EXAMPLE)

target_link_libraries(hello_triangle PUBLIC
    WindowController ShaderCreator ShaderProgram)
target_link_libraries(hello_shaders PUBLIC
    WindowController ShaderCreator ShaderProgram)
target_link_libraries(hello_texture PUBLIC
    WindowController ShaderCreator ShaderProgram)
