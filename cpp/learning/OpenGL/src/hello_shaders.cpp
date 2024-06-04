#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window_tools/WindowLoader.h"
#include "shader_tools/ShaderCreator.h"
#include "shader_tools/ShaderProgram.h"

#include <iostream>
#include <vector>
#include <cmath>

static size_t example {1};

// Changing styles for testing.
static std::vector<GLenum> styles {GL_TRIANGLES, GL_TRIANGLE_FAN,
    GL_TRIANGLE_STRIP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINE_LOOP,
    GL_POINTS};
static size_t current_style_index {0};

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
#if _DEBUG
    std::cout << "Window resized to " << width << ", " << height << ".\n";
#endif
    glViewport(0, 0, width, height);
}

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

static void create_triangle_with_color_attributes(
    struct SimpleTriangle& triangle, const std::vector<GLfloat> vertices)
{
    triangle.vertices = vertices;

    glGenVertexArrays(1, &triangle.vao);
    glGenBuffers(1, &triangle.vbo);

    glBindVertexArray(triangle.vao);
    glBindBuffer(GL_ARRAY_BUFFER, triangle.vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]),
        vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(vertices[0]),
        (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(vertices[0]),
        (void*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
}

static void delete_simple_triangle(struct SimpleTriangle& triangle)
{
    glDeleteVertexArrays(1, &triangle.vao);
    glDeleteBuffers(1, &triangle.vbo);
}

static void make_red_triangle()
{
    struct SimpleTriangle triangle {};
    create_simple_triangle(triangle, std::vector<GLfloat>  {
            0.0f,  0.5f,
            0.5f, -0.5f,
            -0.5f, -0.5f});

    const char* vertex_shader_src {"#version 330 core\n"
        "layout (location = 0) in vec2 pos;\n"
        "out vec4 vertex_color;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(-pos, 0.0, 1.0);\n"
        "   vertex_color = vec4(0.9, 0.0, 0.0, 0.2);\n"
        "}\n"};
    const char* fragment_shader_src {"#version 330 core\n"
        "out vec4 color;\n"
        "in vec4 vertex_color;\n"
        "void main()\n"
        "{\n"
        "   color = vertex_color;"
        "}\n"};

    const GLuint vertex_shader {Marvin::ShaderCreator::create_from_string(
        GL_VERTEX_SHADER, &vertex_shader_src)};
    const GLuint fragment_shader {Marvin::ShaderCreator::create_from_string(
        GL_FRAGMENT_SHADER, &fragment_shader_src)};

    const Marvin::ShaderProgram shader_program { vertex_shader,
        fragment_shader };

    shader_program.use();
    glBindVertexArray(triangle.vao);
    glDrawArrays(styles[current_style_index], 0, 3);

    delete_simple_triangle(triangle);
}

static void make_green_glowing_triangle()
{
    struct SimpleTriangle triangle {};
    create_simple_triangle(triangle, std::vector<GLfloat>  {
            0.0f,  0.5f,
            0.5f, -0.5f,
            -0.5f, -0.5f});

    const char* vertex_shader_src {"#version 330 core\n"
        "layout (location = 0) in vec2 pos;\n"
        "out vec4 vertex_color;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(pos, 0.0, 1.0);\n"
        "   vertex_color = vec4(0.9, 0.0, 0.0, 0.2);\n"
        "}\n"};
    const char* fragment_shader_src {"#version 330 core\n"
        "out vec4 color;\n"
        "uniform vec4 vertex_color;\n"
        "void main()\n"
        "{\n"
        "   color = vertex_color;"
        "}\n"};

    const GLuint vertex_shader {Marvin::ShaderCreator::create_from_string(
        GL_VERTEX_SHADER, &vertex_shader_src)};
    const GLuint fragment_shader {Marvin::ShaderCreator::create_from_string(
        GL_FRAGMENT_SHADER, &fragment_shader_src)};

    const Marvin::ShaderProgram shader_program { vertex_shader,
        fragment_shader };

    shader_program.use();

    const double t {glfwGetTime()};
    const auto value {std::sin(t) / 2.0 + 0.5};
    const GLint vertex_color_location {glGetUniformLocation(
        shader_program.get_id(), "vertex_color")};
    glUniform4f(vertex_color_location, 0.0f, static_cast<GLfloat>(value), 0.0f,
        1.0f);

    glBindVertexArray(triangle.vao);
    glDrawArrays(styles[current_style_index], 0, 3);

    delete_simple_triangle(triangle);
}

static void make_colourful_triangle()
{
    struct SimpleTriangle triangle {};
    create_triangle_with_color_attributes(triangle, std::vector<GLfloat>  {
            0.0f,  0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
           -0.5f, -0.5f, 0.0f, 0.0f, 1.0f});

    const char* vertex_shader_src {"#version 330 core\n"
        "layout (location = 0) in vec2 pos;\n"
        "layout (location = 1) in vec3 col;\n"
        "out vec3 vertex_color;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(pos, 0.0, 1.0);\n"
        "   vertex_color = vec3(pos, 0.0);\n"
        "}\n"};
    const char* fragment_shader_src {"#version 330 core\n"
        "out vec4 color;\n"
        "in vec3 vertex_color;\n"
        "void main()\n"
        "{\n"
        "   color = vec4(vertex_color, 1.0);"
        "}\n"};

    const GLuint vertex_shader {Marvin::ShaderCreator::create_from_string(
        GL_VERTEX_SHADER, &vertex_shader_src)};
    const GLuint fragment_shader {Marvin::ShaderCreator::create_from_string(
        GL_FRAGMENT_SHADER, &fragment_shader_src)};

    const Marvin::ShaderProgram shader_program { vertex_shader,
        fragment_shader };

    shader_program.use();
    glBindVertexArray(triangle.vao);
    glDrawArrays(styles[current_style_index], 0, 3);

    delete_simple_triangle(triangle);
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
    while(!glfwWindowShouldClose(window_loader.get_pointer()))
    {
        render_window_start();

        switch (example)
        {
            case 1:
                make_red_triangle();
                break;

            case 2:
                make_green_glowing_triangle();
                break;

            case 3:
                make_colourful_triangle();
                break;

            default:
                example = 1;
                break;
        }

        render_window_end(window_loader.get_pointer());
    }

    glfwTerminate();

    return 0;
}
