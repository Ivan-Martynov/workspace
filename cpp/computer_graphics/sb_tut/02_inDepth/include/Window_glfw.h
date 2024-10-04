/*
 * Test class for glfw window
 */
#ifndef WINDOW_GLFW_H
#define WINDOW_GLFW_H

#include <memory>
#include <GLFW/glfw3.h>

struct window_deleter {
    void operator()(GLFWwindow* window) { glfwDestroyWindow(window); }
};

class Window_glfw {
private:
    std::unique_ptr<GLFWwindow, window_deleter> window_;
public:
    explicit Window_glfw(const GLuint width, const GLuint height,
            const char* title, GLFWmonitor* monitor, GLFWwindow* share) :
        window_ {std::unique_ptr<GLFWwindow, window_deleter> {
                    glfwCreateWindow(width, height, title, monitor, share),
                    window_deleter()}} { }

    explicit Window_glfw(Window_glfw&) = default;
    Window_glfw& operator=(Window_glfw&) = default;
    explicit Window_glfw(Window_glfw&&) = default;
    Window_glfw& operator=(Window_glfw&&) = default;

    ~Window_glfw() = default;

    GLFWwindow* get_window() const { return window_.get(); }
};

#endif /* WINDOW_GLFW_H */

