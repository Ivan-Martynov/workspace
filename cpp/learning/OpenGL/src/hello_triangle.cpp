//#include <glad/glad.h>
//#include <GLFW/glfw3.h>

#include "window_tools/WindowLoader.h"
#include "shader_tools/ShaderCreator.h"
#include "shader_tools/ShaderProgram.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
#if _DEBUG
    std::cout << "Window resized to " << width << ", " << height << ".\n";
#endif
    glViewport(0, 0, width, height);
}

static GLenum current_mode {GL_FILL};

void key_callback(GLFWwindow* window_ptr, const int key, const int,
        const int action, const int)
{
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS))
    {
        glfwSetWindowShouldClose(window_ptr, GLFW_TRUE);
    }
    else if ((key == GLFW_KEY_SPACE) && (action == GLFW_PRESS))
    {
        current_mode = (current_mode == GL_FILL) ? GL_LINE : GL_FILL;
        glPolygonMode(GL_FRONT_AND_BACK, current_mode);
    }
}

// Process inputs.
void process_input(GLFWwindow* window_ptr)
{
    if (glfwGetKey(window_ptr, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window_ptr, true);
    }
    else if (glfwGetKey(window_ptr, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        current_mode = (current_mode == GL_FILL) ? GL_LINE : GL_FILL;
        glPolygonMode(GL_FRONT_AND_BACK, current_mode);
    }
}

//void update_window(GLFWwindow* window_ptr)
//{
//    process_input(window_ptr);
//}

void render_window_start()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void render_window_end(GLFWwindow* window_ptr)
{
    glfwSwapBuffers(window_ptr);
    glfwPollEvents();    
}

int main()
{
    Marvin::WindowLoader window_loader {};
    if (!window_loader.loaded())
    {
        return 1;
    }

    window_loader.set_resize_callback(framebuffer_size_callback);
    window_loader.set_key_callback(key_callback);

#if 1
    constexpr float vertices[] = {
         0.5f,  0.5f,
         0.5f, -0.5f,
        -0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f,  0.5f,
    };
#else
    constexpr GLfloat vertices[] =
    {
        0.02f, 0.5f, 0.0f, 1.0f,
        1.0f, 0.5f, 0.0f, 1.0f,
        0.9f, 0.6f, 0.0f, 1.0f
    };
#endif

#if 1
    constexpr int indices[] = {
        0, 1, 3, // First triangle.
        1, 2, 3, // Second triangle.
    };
#endif

    const char* vertex_shader_src = "#version 330 core\n"
        "layout (location = 0) in vec2 pos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(pos, 0.0, 1.0);\n"
        "}\n";
    const char* fragment_shader_src = "#version 330 core\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
        "}\n";

    const GLuint vertex_shader {Marvin::ShaderCreator::create_shader(
        GL_VERTEX_SHADER, &vertex_shader_src)};
    const GLuint fragment_shader {Marvin::ShaderCreator::create_shader(
        GL_FRAGMENT_SHADER, &fragment_shader_src)};

    const Marvin::ShaderProgram shader_program { vertex_shader,
        fragment_shader };

    unsigned int vbo {};
    glGenBuffers(1, &vbo);

    unsigned int vao {};
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

#if 1
    unsigned int ebo {};
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
        GL_STATIC_DRAW);
#endif

#if 1
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
        (void*)0);
#else
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
        (void*)0);
#endif

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    while(!glfwWindowShouldClose(window_loader.get_pointer()))
    {
        render_window_start();

        shader_program.use();
        //glBindVertexArray(vao);
        //glDrawArrays(GL_LINE_STRIP_ADJACENCY, 0, 3);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawArrays(GL_LINE_STRIP, 0, 5);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);

        render_window_end(window_loader.get_pointer());
        //glFlush();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    //glDeleteBuffers(1, &ebo);

    glfwTerminate();

    return 0;
}
