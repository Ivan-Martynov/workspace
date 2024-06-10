#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "window_tools/WindowLoader.h"
#include "shader_tools/ShaderCreator.h"
#include "shader_tools/ShaderProgram.h"

#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <filesystem>

static std::filesystem::path assets_path {};
static std::filesystem::path graphics_path {};

static GLfloat ratio {0.25f};
static bool flip_texture {false};
static size_t example {1};

static std::vector<std::array<GLenum, 2>> texture_wrap {
    std::array<GLenum, 2> {GL_REPEAT, GL_REPEAT},
    std::array<GLenum, 2> {GL_REPEAT, GL_MIRRORED_REPEAT},
    std::array<GLenum, 2> {GL_REPEAT, GL_CLAMP_TO_EDGE},
    std::array<GLenum, 2> {GL_REPEAT, GL_CLAMP_TO_BORDER},
    std::array<GLenum, 2> {GL_MIRRORED_REPEAT, GL_REPEAT},
    std::array<GLenum, 2> {GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT},
    std::array<GLenum, 2> {GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE},
    std::array<GLenum, 2> {GL_MIRRORED_REPEAT, GL_CLAMP_TO_BORDER},
    std::array<GLenum, 2> {GL_CLAMP_TO_EDGE, GL_REPEAT},
    std::array<GLenum, 2> {GL_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT},
    std::array<GLenum, 2> {GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE},
    std::array<GLenum, 2> {GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER},
    std::array<GLenum, 2> {GL_CLAMP_TO_BORDER, GL_REPEAT},
    std::array<GLenum, 2> {GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT},
    std::array<GLenum, 2> {GL_CLAMP_TO_BORDER, GL_CLAMP_TO_EDGE},
    std::array<GLenum, 2> {GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER},
};

// Changing styles for testing.
//static std::vector<GLenum> styles {GL_TRIANGLES, GL_TRIANGLE_FAN,
//    GL_TRIANGLE_STRIP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINE_LOOP,
//    GL_POINTS};
static size_t current_style_index {0};

static std::vector<GLenum> vec {GL_TRIANGLES, GL_TRIANGLE_FAN,
    GL_TRIANGLE_STRIP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINE_LOOP,
    GL_POINTS};

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

    if (action == GLFW_REPEAT)
    {
        switch (key)
        {
            case GLFW_KEY_DOWN:
                ratio = std::max(0.0f, ratio - 0.05f);
                break;

            case GLFW_KEY_UP:
                ratio = std::min(1.0f, ratio + 0.05f);
                break;
        }
    }

    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window_ptr, GLFW_TRUE);
                break;

            case GLFW_KEY_SPACE:
                if (current_mode == GL_FILL)
                {
                    current_mode = GL_LINE;
                }
                else if (current_mode == GL_LINE)
                {
                    current_mode = GL_POINT;
                }
                else
                {
                    current_mode = GL_FILL;
                }
                glPolygonMode(GL_FRONT_AND_BACK, current_mode);
                break;

            case GLFW_KEY_LEFT:
                if (current_style_index == 0)
                {
                    current_style_index = texture_wrap.size() - 1;
                }
                else
                {
                    --current_style_index;
                }
                break;

            case GLFW_KEY_RIGHT:
                current_style_index = (current_style_index + 1) %
                    texture_wrap.size();
                break;

            case GLFW_KEY_S:
            case GLFW_KEY_W:
                flip_texture = !flip_texture;
                break;

#if 1
            case GLFW_KEY_DOWN:
                ratio = std::max(0.0f, ratio - 0.05f);
                break;

            case GLFW_KEY_UP:
                ratio = std::min(1.0f, ratio + 0.05f);
                break;
#endif

            case GLFW_KEY_1:
                example = 1;
                break;

            case GLFW_KEY_2:
                example = 2;
                break;

            case GLFW_KEY_3:
                example = 3;
                break;

            case GLFW_KEY_4:
                example = 4;
                break;

            case GLFW_KEY_5:
                example = 5;
                break;

            case GLFW_KEY_6:
                example = 6;
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

#if 0
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
#endif

static void load_texture_example()
{
    constexpr GLfloat vertices[] {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    constexpr GLuint indices[] {
        0, 1, 3,
        1, 2, 3
    };

    GLuint vao {};
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo {};
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo {};
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
        (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
        (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

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
        "   texture_coord = vec2(tex.xy);\n"
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

    GLuint texture_id {};
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(flip_texture);

    //std::filesystem::path file_path {"assets/graphics/brick_wall.jpg"};
    std::filesystem::path file_path {graphics_path / "brick_wall.jpg"};
    int width {};
    int height {};
    int channel_count {};
    unsigned char* image_data {stbi_load(file_path.string().c_str(),
        &width, &height, &channel_count, 0)};

    if (image_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(image_data);
    }
    else
    {
        std::cerr << "Failed to load texture " << file_path << "\n";
    }

    glBindTexture(GL_TEXTURE_2D, texture_id);

    shader_program.use();

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

static void load_disco_container()
{
    constexpr GLfloat vertices[] {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    constexpr GLuint indices[] {
        0, 1, 3,
        1, 2, 3
    };

    GLuint vao {};
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo {};
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo {};
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
        (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
        (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

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
        "   texture_coord = vec2(tex.xy);\n"
        "}\n"};
    const char* fragment_shader_src {"#version 330 core\n"
        "out vec4 color;\n"
        "in vec3 vertex_color;\n"
        "in vec2 texture_coord;\n"
        "uniform sampler2D tex;\n"
        "void main()\n"
        "{\n"
        "   color = texture(tex, texture_coord) * vec4(vertex_color, 1.0);\n"
        "}\n"};

    const GLuint vertex_shader {Marvin::ShaderCreator::create_from_string(
        GL_VERTEX_SHADER, &vertex_shader_src)};
    const GLuint fragment_shader {Marvin::ShaderCreator::create_from_string(
        GL_FRAGMENT_SHADER, &fragment_shader_src)};

    const Marvin::ShaderProgram shader_program { vertex_shader,
        fragment_shader };

    GLuint texture_id {};
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(flip_texture);

    std::filesystem::path file_path {graphics_path / "container.jpg"};
    int width {};
    int height {};
    int channel_count {};
    unsigned char* image_data {stbi_load(file_path.string().c_str(),
        &width, &height, &channel_count, 0)};

    if (image_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(image_data);
    }
    else
    {
        std::cerr << "Failed to load texture " << file_path << "\n";
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    shader_program.use();

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

static void load_texture_with_face()
{
    constexpr GLfloat vertices[] {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    constexpr GLuint indices[] {
        0, 1, 3,
        1, 2, 3
    };

    GLuint vao {};
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo {};
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo {};
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
        (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
        (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

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
        "   texture_coord = vec2(tex.xy);\n"
        "}\n"};
    const char* fragment_shader_src {"#version 330 core\n"
        "out vec4 color;\n"
        "in vec3 vertex_color;\n"
        "in vec2 texture_coord;\n"
        "uniform sampler2D tex1;\n"
        "uniform sampler2D tex2;\n"
        "void main()\n"
        "{\n"
        "   color = mix(texture(tex1, texture_coord),"
        "               texture(tex2, texture_coord), 0.2);\n"
        "}\n"};

    const GLuint vertex_shader {Marvin::ShaderCreator::create_from_string(
        GL_VERTEX_SHADER, &vertex_shader_src)};
    const GLuint fragment_shader {Marvin::ShaderCreator::create_from_string(
        GL_FRAGMENT_SHADER, &fragment_shader_src)};

    const Marvin::ShaderProgram shader_program { vertex_shader,
        fragment_shader };

    GLuint texture_id {};
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(flip_texture);

    std::filesystem::path file_path {graphics_path / "brick_wall.jpg"};
    int width {};
    int height {};
    int channel_count {};
    unsigned char* image_data {stbi_load(file_path.string().c_str(),
        &width, &height, &channel_count, 0)};

    if (image_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(image_data);
    }
    else
    {
        std::cerr << "Failed to load texture " << file_path << "\n";
    }

    GLuint texture_id2 {};
    glGenTextures(1, &texture_id2);
    glBindTexture(GL_TEXTURE_2D, texture_id2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    std::filesystem::path file_path2 {graphics_path / "awesomeface.png"};
    unsigned char* image_data2 {stbi_load(file_path2.string().c_str(),
        &width, &height, &channel_count, 0)};

    if (image_data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, image_data2);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(image_data2);
    }
    else
    {
        std::cerr << "Failed to load texture " << file_path2 << "\n";
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_id2);

    shader_program.use();
    glUniform1i(glGetUniformLocation(shader_program.get_id(), "tex1"), 0);
    glUniform1i(glGetUniformLocation(shader_program.get_id(), "tex2"), 1);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

static void load_texture_with_face_looking_other_way()
{
    constexpr GLfloat vertices[] {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    constexpr GLuint indices[] {
        0, 1, 3,
        3, 2, 1
    };

    GLuint vao {};
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo {};
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo {};
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
        (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
        (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

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
        "   texture_coord = vec2(tex.xy);\n"
        "}\n"};
    const char* fragment_shader_src {"#version 330 core\n"
        "out vec4 color;\n"
        "in vec3 vertex_color;\n"
        "in vec2 texture_coord;\n"
        "uniform sampler2D tex1;\n"
        "uniform sampler2D tex2;\n"
        "void main()\n"
        "{\n"
        "   color = mix(texture(tex1, texture_coord),"
        "               texture(tex2, "
        "               vec2(1 - texture_coord.x, texture_coord.y)), 0.2);\n"
        "}\n"};

    const GLuint vertex_shader {Marvin::ShaderCreator::create_from_string(
        GL_VERTEX_SHADER, &vertex_shader_src)};
    const GLuint fragment_shader {Marvin::ShaderCreator::create_from_string(
        GL_FRAGMENT_SHADER, &fragment_shader_src)};

    const Marvin::ShaderProgram shader_program { vertex_shader,
        fragment_shader };

    GLuint texture_id {};
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(flip_texture);

    std::filesystem::path file_path {graphics_path / "brick_wall.jpg"};
    int width {};
    int height {};
    int channel_count {};
    unsigned char* image_data {stbi_load(file_path.string().c_str(),
        &width, &height, &channel_count, 0)};

    if (image_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(image_data);
    }
    else
    {
        std::cerr << "Failed to load texture " << file_path << "\n";
    }

    GLuint texture_id2 {};
    glGenTextures(1, &texture_id2);
    glBindTexture(GL_TEXTURE_2D, texture_id2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::filesystem::path file_path2 {graphics_path / "awesomeface.png"};
    unsigned char* image_data2 {stbi_load(file_path2.string().c_str(),
        &width, &height, &channel_count, 0)};

    if (image_data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, image_data2);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(image_data2);
    }
    else
    {
        std::cerr << "Failed to load texture " << file_path2 << "\n";
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_id2);

    shader_program.use();
    glUniform1i(glGetUniformLocation(shader_program.get_id(), "tex1"), 0);
    glUniform1i(glGetUniformLocation(shader_program.get_id(), "tex2"), 1);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

static void load_texture_different_options()
{
    constexpr GLfloat vertices[] {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // top left 
    };

    constexpr GLuint indices[] {
        0, 1, 3,
        3, 2, 1
    };

    GLuint vao {};
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo {};
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo {};
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
        (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
        (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

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
        "uniform sampler2D tex1;\n"
        "uniform sampler2D tex2;\n"
        "uniform float ratio;\n"
        "void main()\n"
        "{\n"
        "   color = mix(texture(tex1, texture_coord),"
        "               texture(tex2, texture_coord), ratio);\n"
        "}\n"};

    const GLuint vertex_shader {Marvin::ShaderCreator::create_from_string(
        GL_VERTEX_SHADER, &vertex_shader_src)};
    const GLuint fragment_shader {Marvin::ShaderCreator::create_from_string(
        GL_FRAGMENT_SHADER, &fragment_shader_src)};

    const Marvin::ShaderProgram shader_program { vertex_shader,
        fragment_shader };

    GLuint texture_id {};
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
        texture_wrap[current_style_index][0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
        texture_wrap[current_style_index][0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(flip_texture);

    std::filesystem::path file_path {graphics_path / "brick_wall.jpg"};
    int width {};
    int height {};
    int channel_count {};
    unsigned char* image_data {stbi_load(file_path.string().c_str(),
        &width, &height, &channel_count, 0)};

    if (image_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(image_data);
    }
    else
    {
        std::cerr << "Failed to load texture " << file_path << "\n";
    }

    GLuint texture_id2 {};
    glGenTextures(1, &texture_id2);
    glBindTexture(GL_TEXTURE_2D, texture_id2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
        texture_wrap[current_style_index][1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
        texture_wrap[current_style_index][1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::filesystem::path file_path2 {graphics_path / "awesomeface.png"};
    unsigned char* image_data2 {stbi_load(file_path2.string().c_str(),
        &width, &height, &channel_count, 0)};

    if (image_data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, image_data2);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(image_data2);
    }
    else
    {
        std::cerr << "Failed to load texture " << file_path2 << "\n";
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_id2);

    shader_program.use();
    glUniform1f(glGetUniformLocation(shader_program.get_id(), "ratio"), ratio);
    glUniform1i(glGetUniformLocation(shader_program.get_id(), "tex1"), 0);
    glUniform1i(glGetUniformLocation(shader_program.get_id(), "tex2"), 1);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

static void load_pixelated_texture()
{
    constexpr GLfloat vertices[] {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.55f, 0.55f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.55f, 0.45f,   // bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.45f, 0.45f,   // bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.45f, 0.55f    // top left 
    };

    constexpr GLuint indices[] {
        0, 1, 3,
        3, 2, 1
    };

    GLuint vao {};
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo {};
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo {};
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
        (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
        (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

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
        "uniform sampler2D tex1;\n"
        "uniform sampler2D tex2;\n"
        "void main()\n"
        "{\n"
        "   color = mix(texture(tex1, texture_coord),"
        "               texture(tex2, texture_coord), 0.25);\n"
        "}\n"};

    const GLuint vertex_shader {Marvin::ShaderCreator::create_from_string(
        GL_VERTEX_SHADER, &vertex_shader_src)};
    const GLuint fragment_shader {Marvin::ShaderCreator::create_from_string(
        GL_FRAGMENT_SHADER, &fragment_shader_src)};

    const Marvin::ShaderProgram shader_program { vertex_shader,
        fragment_shader };

    GLuint texture_id {};
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(flip_texture);

    std::filesystem::path file_path {graphics_path / "brick_wall.jpg"};
    int width {};
    int height {};
    int channel_count {};
    unsigned char* image_data {stbi_load(file_path.string().c_str(),
        &width, &height, &channel_count, 0)};

    if (image_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(image_data);
    }
    else
    {
        std::cerr << "Failed to load texture " << file_path << "\n";
    }

    GLuint texture_id2 {};
    glGenTextures(1, &texture_id2);
    glBindTexture(GL_TEXTURE_2D, texture_id2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    std::filesystem::path file_path2 {graphics_path / "awesomeface.png"};
    unsigned char* image_data2 {stbi_load(file_path2.string().c_str(),
        &width, &height, &channel_count, 0)};

    if (image_data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, image_data2);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(image_data2);
    }
    else
    {
        std::cerr << "Failed to load texture " << file_path2 << "\n";
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_id2);

    shader_program.use();
    glUniform1i(glGetUniformLocation(shader_program.get_id(), "tex1"), 0);
    glUniform1i(glGetUniformLocation(shader_program.get_id(), "tex2"), 1);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

int main(int, char* argv[])
{
    Marvin::WindowLoader window_loader {900, 600, "Hello, texture."};
    if (!window_loader.loaded())
    {
        return 1;
    }

    // Full path of the executable.
    assets_path = std::filesystem::canonical(argv[0]);
    // We know the location is in ./bin/debug/ or ./bin/release/ which is given
    // via parent_path. Therefore, two more calls to parent_path are needed in
    // order to get to the root directory.
    for (size_t i {0}; i < 3; ++i)
    {
        assets_path = assets_path.parent_path();
    }

    assets_path /= "assets";
    graphics_path = assets_path / "graphics";

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
                load_disco_container();
                break;

            case 3:
                load_texture_with_face();
                break;

            case 4:
                load_texture_with_face_looking_other_way();
                break;

            case 5:
                load_texture_different_options();
                break;

            case 6:
                load_pixelated_texture();
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
