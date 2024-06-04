#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

static void load_texture_example()
{
    GLuint texture_id {};
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    constexpr char* file_name {"container.jpg"};
    int width {};
    int height {};
    int channel_count {};
    unsigned char* image_data {stbi_load(file_name, &width, &height,
        &channel_count, 0)};

    if (image_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(image_data);
    }
    else
    {
        std::cerr << "Failed to load texture " << file_name << "\n";
        return;
    }

    constexpr float vertices[] {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
        (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    const char* vertex_shader_src {"#version 330 core\n"
        "layout (location = 0) in vec3 pos;\n"
        "layout (location = 1) in vec3 col;\n"
        "layout (location = 2) in vec2 tex;\n"
        "out vec3 vertex_color;\n"
        "out vec2 texture_coord;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(pos, 1.0);\n"
        "   vertex_color = col;\n"
        "   texture_coord = tex;\n"
        "}\n"};
    const char* fragment_shader_src {"#version 330 core\n"
        "out vec4 color;\n"
        "in vec3 vertex_color;\n"
        "in vec2 texture_coord;\n"
        "uniform sampler2D tex;\n"
        "void main()\n"
        "{\n"
        "   color = texture(tex, texture_coord);\n"
        "}\n"};

    const GLuint vertex_shader {Marvin::ShaderCreator::create_from_string(
        GL_VERTEX_SHADER, &vertex_shader_src)};
    const GLuint fragment_shader {Marvin::ShaderCreator::create_from_string(
        GL_FRAGMENT_SHADER, &fragment_shader_src)};

    const Marvin::ShaderProgram shader_program { vertex_shader,
        fragment_shader };


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
                load_texture_example();
                break;

            case 2:
                break;

            case 3:
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
