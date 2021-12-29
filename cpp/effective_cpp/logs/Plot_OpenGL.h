#ifndef PLOT_OPENGL_H
#define PLOT_OPENGL_H

// std library to handle exceptions
#include <stdexcept>
// OpenGL libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Plot2D_OpenGL {
public:
    Plot2D_OpenGL() = default;
    Plot2D_OpenGL(const GLuint w, const GLuint h): win_{init_window(w, h)} {
        //init_window(w, h);
    }

    explicit Plot2D_OpenGL(const Plot2D_OpenGL&) = default;
    Plot2D_OpenGL& operator=(const Plot2D_OpenGL&) = default;
    explicit Plot2D_OpenGL(Plot2D_OpenGL&&) = default;
    Plot2D_OpenGL& operator=(Plot2D_OpenGL&&) = default;

    ~Plot2D_OpenGL() { glfwTerminate(); }

    void plot() {
        // check if GLFW has been told to close
        while (!glfwWindowShouldClose(win_)) {
            glfwPollEvents(); // check for triggered events
            // --- Rendering happens here
            // Set the color, clear the color buffer
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            // swap the screen buffers
            glfwSwapBuffers(win_);
        }
    }

private:
    GLFWwindow* win_;
    GLFWwindow* init_window(const GLuint, const GLuint);
};


/*
 * Function to initialize stuff. With comparison to the first tutorial here we
 * put more stuff inside a single function, though trying not to make it too
 * long (keeping the idea of fitting a single function on a screen in mind)
 */
GLFWwindow* Plot2D_OpenGL::init_window(const GLuint w, const GLuint h) {
    glfwInit();

    // --- configuring GLFW ---
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // --- create a window object
    GLFWwindow *win = glfwCreateWindow(w, h, "Window", nullptr, nullptr);
    if (win == nullptr)
        throw std::runtime_error {"Failed to create GLFW window"};

    glfwMakeContextCurrent(win);

    // let GLEW know to use a modern approach to retrieving function pointers
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        throw std::runtime_error {"Failed to initialize GLEW"};

    // register the callback
    //glfwSetKeyCallback(win, key_callback);

    // inform OpenGL about the size of the rendering window
    glViewport(0, 0, w, h);

    return win;
}


#endif /* PLOT_OPENGL_H */

