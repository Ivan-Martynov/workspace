#ifndef OPENGL_BASE_H
#define OPENGL_BASE_H

#include <sstream>
#include <iostream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

// GL math
//#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace OpenGL_tutorial {

class OpenGL_base {
private:
    void virtual show_menu(const std::string&);
    void virtual process_input(const std::string&, const std::string&);
    void virtual game_loop(const int = 0);
    int virtual clean_up(const int);

protected:
    GLFWwindow* window_ {};

    static void error_callback(const int, const char*);
    static void key_callback(GLFWwindow*, const int, const int, const int,
            const int);
    static void resize_callback(GLFWwindow*, const int, const int);
    static void window_refresh_callback(GLFWwindow*);

public:
    explicit OpenGL_base(const GLuint, const GLuint, const std::string&);
    explicit OpenGL_base(const GLuint, const GLuint, const std::string&,
            GLFWmonitor*, GLFWwindow*);
    explicit OpenGL_base(const GLuint, const GLuint, const std::string&,
            GLFWmonitor*, GLFWwindow*, const int, const int);

    explicit OpenGL_base(const OpenGL_base&) = delete;
    OpenGL_base& operator=(const OpenGL_base&) = delete;
    explicit OpenGL_base(OpenGL_base&&) = delete;
    OpenGL_base& operator=(OpenGL_base&&) = delete;

    virtual ~OpenGL_base() = default;

    int virtual run(const int, const char*[]);
};

OpenGL_base::OpenGL_base(const GLuint width, const GLuint height,
        const std::string& title, GLFWmonitor* monitor, GLFWwindow* share,
        const int majorVersion, const int minorVersion) {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        throw std::runtime_error {"Failed to initialize GLFW"};

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(width, height, title.c_str(), monitor, share);

    if (window_ == nullptr)
        throw std::runtime_error {"Failed to create GLFW window"};

    glfwSetKeyCallback(window_, key_callback);
    glfwSetWindowSizeCallback(window_, resize_callback);

    glfwMakeContextCurrent(window_);

    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    glViewport(0, 0, width, height);
}

OpenGL_base::OpenGL_base(const GLuint width, const GLuint height,
        const std::string& title, GLFWmonitor* monitor, GLFWwindow* share) :
    OpenGL_base(width, height, title, monitor, share, 4, 5) {
    }

OpenGL_base::OpenGL_base(const GLuint width, const GLuint height,
        const std::string& title) :
    OpenGL_base(width, height, title, nullptr, nullptr) {
    }

void OpenGL_base::error_callback(const int, const char* description)
{
    std::cerr << "Error: " << description << '\n';
}

void OpenGL_base::key_callback(GLFWwindow* window, const int key, const int,
        const int action, const int mods)
{
    if (key == GLFW_KEY_Q && action == GLFW_PRESS && mods == GLFW_MOD_CONTROL)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void OpenGL_base::resize_callback(GLFWwindow*, const int w, const int h) {
    glViewport(0, 0, w, h);
}

void OpenGL_base::window_refresh_callback(GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
}

void OpenGL_base::show_menu(const std::string& program_name) {
    std::cout << "Note: the program can be run as follows:\n" <<
        program_name << '\n';
    game_loop();
}

void OpenGL_base::process_input(const std::string& input,
        const std::string& program_name) {
    std::istringstream ss {input};
    int x;
    if (ss >> x) {
        switch (x) {
            default:
                goto wrong_input;
        }
    }

wrong_input:
    std::cerr << "Wrong input: drawing default window\n";
    show_menu(program_name);
}

int OpenGL_base::clean_up(const int val) {
    glfwDestroyWindow(window_);
    glfwTerminate();
    return val;
}

int OpenGL_base::run(const int argc, const char* argv[]) try {

    if (argc > 1)
        process_input(argv[1], argv[0]);
    else
        show_menu(argv[0]);

    return 0;
} catch (const std::runtime_error &e) {
    std::cerr << e.what() << '\n';
    return clean_up(1);
} catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return clean_up(2);
} catch (...) {
    std::cerr << "Unknown exception\n";
    return clean_up(3);
}

void OpenGL_base::game_loop(const int) {

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();
        glfwSwapBuffers(window_);
    }

    glfwDestroyWindow(window_);
    glfwTerminate();
}

}

#endif /* OPENGL_BASE_H */

