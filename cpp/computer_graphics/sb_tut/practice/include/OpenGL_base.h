#ifndef OPENGL_BASE_H
#define OPENGL_BASE_H

#include <sstream>
#include <iostream>
#include <memory>

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
    void virtual start_up();
    void virtual render(const int, const double = 0);
    void virtual shutdown();
    int virtual clean_up(const int);

    static void error_callback(const int, const char*);
    static void key_callback(GLFWwindow*, const int, const int, const int,
            const int);
    static void resize_callback(GLFWwindow*, const int, const int);
    static void window_refresh_callback(GLFWwindow*);

protected:
    static std::unique_ptr<OpenGL_base> app_ptr_;
    GLFWwindow* window_ {};
    void virtual game_loop(const int = 0);

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

    virtual void on_error(const char*);
    virtual void on_key(const int, const int, const int, const int);
    virtual void on_resize(const int, const int);
    virtual void on_refresh();

    virtual int run(std::unique_ptr<OpenGL_base>&&, const int, const char*[]);
};

std::unique_ptr<OpenGL_base> OpenGL_base::app_ptr_ = nullptr;

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

void OpenGL_base::on_error(const  char* description) {
    std::cerr << "Error: " << description << '\n';
}

void OpenGL_base::on_key(const int key, const int action, const int,
        const int mods) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS && mods == GLFW_MOD_CONTROL)
        glfwSetWindowShouldClose(window_, GLFW_TRUE);
}

void OpenGL_base::on_resize(const int w, const int h) {
    glViewport(0, 0, w, h);
}

void OpenGL_base::on_refresh() {
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window_);
}

void OpenGL_base::error_callback(const int, const char* description)
{
    app_ptr_->on_error(description);
}

void OpenGL_base::key_callback(GLFWwindow*, const int key, const int scan,
        const int action, const int mods)
{
    app_ptr_->on_key(key, action, scan, mods);
}

void OpenGL_base::resize_callback(GLFWwindow*, const int w, const int h) {
    app_ptr_->on_resize(w, h);
}

void OpenGL_base::window_refresh_callback(GLFWwindow*) {
    app_ptr_->on_refresh();
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

int OpenGL_base::run(std::unique_ptr<OpenGL_base>&& app, const int argc,
        const char* argv[]) try {
    app_ptr_ = std::move(app);

    if (argc > 1)
        process_input(argv[1], argv[0]);
    else
        show_menu(argv[0]);

    return 0;
} catch (const std::runtime_error& e) {
    std::cerr << e.what() << '\n';
    return clean_up(1);
} catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return clean_up(2);
} catch (...) {
    std::cerr << "Unknown exception\n";
    return clean_up(3);
}

void OpenGL_base::start_up() { }

void OpenGL_base::render(const int, const double) { }

void OpenGL_base::shutdown() { }

void OpenGL_base::game_loop(const int option) {

    start_up();

    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        render(option);

        glfwSwapBuffers(window_);
    }

    shutdown();

    glfwDestroyWindow(window_);
    glfwTerminate();
}

}

#endif /* OPENGL_BASE_H */

