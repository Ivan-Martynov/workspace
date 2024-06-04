#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window_tools/WindowLoader.h"
#include "shader_tools/ShaderCreator.h"
#include "shader_tools/ShaderProgram.h"

#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
#if _DEBUG
    std::cout << "Window resized to " << width << ", " << height << ".\n";
#endif
    glViewport(0, 0, width, height);
}


// Switching to different examples.
static size_t example {3};

// Changing styles for testing.
static std::vector<GLenum> styles {GL_TRIANGLES, GL_TRIANGLE_FAN,
    GL_TRIANGLE_STRIP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINE_LOOP,
    GL_POINTS};
static size_t current_style_index {0};

void key_callback(GLFWwindow* window_ptr, const int key, const int,
        const int action, const int)
{
    static GLenum current_mode {GL_FILL};

    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window_ptr, GLFW_TRUE);
                break;

            case GLFW_KEY_SPACE:
                current_mode = (current_mode == GL_FILL) ? GL_LINE : GL_FILL;
                glPolygonMode(GL_FRONT_AND_BACK, current_mode);
                break;

            case GLFW_KEY_LEFT:
                if (current_style_index == 0)
                {
                    current_style_index = styles.size() - 1;
                }
                else
                {
                    --current_style_index;
                }
                break;

            case GLFW_KEY_RIGHT:
                current_style_index = (current_style_index + 1) % styles.size();
                break;

            case GLFW_KEY_1:
                example = 1;
                break;

            case GLFW_KEY_2:
                example = 2;
                break;

            case GLFW_KEY_3:
                example = 3;
                break;
        }
    }
}

void render_window_start()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void render_window_end(GLFWwindow* window_ptr)
{
    glfwSwapBuffers(window_ptr);
    glfwPollEvents();    
    //glFlush();
}

struct SimpleTriangle
{
    std::vector<GLfloat> vertices;
    GLuint vbo;
    GLuint vao;
};

static void create_simple_triangle(struct SimpleTriangle& triangle,
    const std::vector<GLfloat> vertices)
{
    triangle.vertices = vertices;

    glGenVertexArrays(1, &triangle.vao);
    glGenBuffers(1, &triangle.vbo);

    glBindVertexArray(triangle.vao);
    glBindBuffer(GL_ARRAY_BUFFER, triangle.vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]),
        vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(vertices[0]),
        (void*)0);
    glEnableVertexAttribArray(0);
}

static void delete_simple_triangle(struct SimpleTriangle& triangle)
{
    glDeleteVertexArrays(1, &triangle.vao);
    glDeleteBuffers(1, &triangle.vbo);
}

static void draw_simple_triangles(Marvin::WindowLoader& window_loader,
    const size_t triangle_count)
{
    const char* vertex_shader_src {"#version 330 core\n"
        "layout (location = 0) in vec2 pos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(pos, 0.0, 1.0);\n"
        "}\n"};
    const char* fragment_shader_src {"#version 330 core\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
        "}\n"};

    const GLuint vertex_shader {Marvin::ShaderCreator::create_from_string(
        GL_VERTEX_SHADER, &vertex_shader_src)};
    const GLuint fragment_shader {Marvin::ShaderCreator::create_from_string(
        GL_FRAGMENT_SHADER, &fragment_shader_src)};

    const Marvin::ShaderProgram shader_program { vertex_shader,
        fragment_shader };

    std::vector<GLfloat> vertices;
    if (triangle_count == 1)
    {
        vertices = {
            0.0f,  0.5f,
            0.5f, -0.5f,
            -0.5f, -0.5f
        };
    }
    else
    {
        vertices = {
            -0.5f, 0.75f,
            0.0f,  0.0f,
            -1.0f,  0.0f,
            0.0f,  0.0f,
            0.5f, 0.75f,
            1.0f,  0.0f,
        };
    }

    GLuint vbo {};
    glGenBuffers(1, &vbo);

    GLuint vao {};
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]),
        vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(vertices[0]),
        (void*)0);

    glEnableVertexAttribArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    while(!glfwWindowShouldClose(window_loader.get_pointer()))
    {
        render_window_start();

        const GLenum style = styles[current_style_index];

        shader_program.use();
        //glBindVertexArray(vao);
        glDrawArrays(style, 0, 3 * triangle_count);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        //glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, nullptr);
        //glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, nullptr);
        //glDrawElements(GL_LINES, 4, GL_UNSIGNED_INT, nullptr);

        render_window_end(window_loader.get_pointer());
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

static void draw_two_triangles(Marvin::WindowLoader& window_loader)
{
    struct SimpleTriangle first_triangle {};
    create_simple_triangle(first_triangle, std::vector<GLfloat>  {
            -0.5f, 0.75f,
            0.0f,  0.0f,
            -1.0f,  0.0f});
    struct SimpleTriangle second_triangle {};
    create_simple_triangle(second_triangle, std::vector<GLfloat>  {
            0.0f,  0.0f,
            0.5f, 0.75f,
            1.0f,  0.0f});

    const char* vertex_shader_src {"#version 330 core\n"
        "layout (location = 0) in vec2 pos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(pos, 0.0, 1.0);\n"
        "}\n"};
    const char* fragment_shader_src {"#version 330 core\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
        "}\n"};
    const char* fragment_shader_src2 {"#version 330 core\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0f, 1.0f, 0.0f, 1.0f);"
        "}\n"};

    const GLuint vertex_shader {Marvin::ShaderCreator::create_from_string(
        GL_VERTEX_SHADER, &vertex_shader_src)};
    const GLuint fragment_shader {Marvin::ShaderCreator::create_from_string(
        GL_FRAGMENT_SHADER, &fragment_shader_src)};

    const Marvin::ShaderProgram shader_program { vertex_shader,
        fragment_shader };
    const Marvin::ShaderProgram shader_program2 { vertex_shader,
        Marvin::ShaderCreator::create_from_string(GL_FRAGMENT_SHADER,
            &fragment_shader_src2) };

    //glEnableVertexAttribArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    while(!glfwWindowShouldClose(window_loader.get_pointer()))
    {
        render_window_start();

        const GLenum style = styles[current_style_index];

        shader_program.use();
        //glBindVertexArray(vao);
        glBindVertexArray(first_triangle.vao);
        glDrawArrays(style, 0, 3);

        shader_program2.use();
        glBindVertexArray(second_triangle.vao);
        glDrawArrays(style, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        //glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, nullptr);
        //glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, nullptr);
        //glDrawElements(GL_LINES, 4, GL_UNSIGNED_INT, nullptr);

        render_window_end(window_loader.get_pointer());
    }

    delete_simple_triangle(first_triangle);
    delete_simple_triangle(second_triangle);
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

    glPointSize(5);

#if 0
    constexpr float vertices[] {
         0.5f,  0.5f,
         0.5f, -0.5f,
        -0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f,  0.5f,
    };
    constexpr GLfloat vertices[]
    {
        0.02f, 0.5f, 0.0f, 1.0f,
        1.0f, 0.5f, 0.0f, 1.0f,
        0.9f, 0.6f, 0.0f, 1.0f
    };
#endif

#if 0
    constexpr int indices[] {
        0, 1, 3, // First triangle.
        1, 2, 3, // Second triangle.
    };
    constexpr int indices[] {
        0, 1, 2, 3
    };

    const char* vertex_shader_src {"#version 330 core\n"
        "layout (location = 0) in vec2 pos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(pos, 0.0, 1.0);\n"
        "}\n"};
    const char* fragment_shader_src {"#version 330 core\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
        "}\n"};

    const GLuint vertex_shader {Marvin::ShaderCreator::create_shader(
        GL_VERTEX_SHADER, &vertex_shader_src)};
    const GLuint fragment_shader {Marvin::ShaderCreator::create_shader(
        GL_FRAGMENT_SHADER, &fragment_shader_src)};

    const Marvin::ShaderProgram shader_program { vertex_shader,
        fragment_shader };

#endif
#if 0
    unsigned int vbo {};
    glGenBuffers(1, &vbo);

    unsigned int vao {};
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo {};
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
        (void*)0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);
#endif

    switch (example)
    {
        case 1:
            draw_simple_triangles(window_loader, 1);
            break;

        case 2:
            draw_simple_triangles(window_loader, 2);
            break;

        case 3:
            draw_two_triangles(window_loader);
            break;

        default:
            break;
    }

    

#if 0
    //glPointSize(5);
    while(!glfwWindowShouldClose(window_loader.get_pointer()))
    {
        render_window_start();

        shader_program.use();
        //glBindVertexArray(vao);
        //glDrawArrays(GL_LINE_STRIP_ADJACENCY, 0, 3);
        //glDrawArrays(GL_LINE_LOOP, 0, 4);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawArrays(GL_LINE_STRIP, 0, 5);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        //glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, nullptr);
        //glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, nullptr);
        //glDrawElements(GL_LINES, 4, GL_UNSIGNED_INT, nullptr);

        render_window_end(window_loader.get_pointer());
    }

#endif
    glfwTerminate();

    return 0;
}
