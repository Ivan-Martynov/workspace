#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback([[maybe_unused]] GLFWwindow* window_ptr,
    int width, int height)
{
#if _DEBUG
    std::cout << "Window resized to " << width << ", " << height << ".\n";
#endif
    glViewport(0, 0, width, height);
}

static GLenum current_mode {GL_FILL};

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

enum class load_codes {
    SUCCESS,
    FAIL_GLFW_INIT,
    FAIL_GLFW_WINDOW_CREATE,
    FAIL_GLAD_INIT,
};

struct WindowLoader
{
    load_codes state;
    GLFWwindow* window_ptr;
};

bool load_window(struct WindowLoader& window_loader)
{
    // Initialize glfw.
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW.\n";
        window_loader.state = load_codes::FAIL_GLFW_INIT;
        return false;
    }

    // Configure glfw.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a window.
    window_loader.window_ptr = glfwCreateWindow(800, 600, "Hello, Window",
        nullptr, nullptr);
    if (!window_loader.window_ptr)
    {
        std::cout << "Failed to create a GLFW window.\n";
        window_loader.state = load_codes::FAIL_GLFW_WINDOW_CREATE;
        return false;
    }
    glfwMakeContextCurrent(window_loader.window_ptr);
    glfwSetFramebufferSizeCallback(window_loader.window_ptr,
        framebuffer_size_callback);

    // Load all OpenGL function pointers.
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD.\n";
        window_loader.state = load_codes::FAIL_GLAD_INIT;
        return false;
    }

    window_loader.state = load_codes::SUCCESS;
    return true;
}

void update_window(GLFWwindow* window_ptr)
{
    process_input(window_ptr);
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
}

void render_window(GLFWwindow* window_ptr)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window_ptr);
    glfwPollEvents();    
}

int main()
{
    struct WindowLoader window_loader {};
    if (!load_window(window_loader))
    {
        return static_cast<int>(window_loader.state);
    }

#if 1
    constexpr float vertices[] = {
         0.5f,  0.5f,
         0.5f, -0.5f,
        -0.5f, -0.5f,
        -0.5f,  0.5f,
    };
    constexpr int indices[] = {
        0, 1, 3, // First triangle.
        1, 2, 3, // Second triangle.
    };
#else
    constexpr float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };
#endif

    const char* vertex_shader_src = "#version 330 core\n"
        "layout (location = 0) in vec2 pos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);\n"
        "}\n";
    const char* fragment_shader_src = "#version 330 core\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
        "}\n";

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_src, nullptr);
    glCompileShader(vertex_shader);

    int success{};
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
        std::cout << "Failed to compile a vertex shader\n";
    }

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_src, nullptr);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
        std::cout << "Failed to compile a fragment shader\n";
    }

    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    unsigned int vbo {};
    glGenBuffers(1, &vbo);

    unsigned int vao {};
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int ebo {};
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
        (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    while(!glfwWindowShouldClose(window_loader.window_ptr))
    {
        update_window(window_loader.window_ptr);

        render_window_start();

        glUseProgram(shader_program);
        //glBindVertexArray(vao);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);

        render_window_end(window_loader.window_ptr);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shader_program);

    glfwTerminate();

    return 0;
}
