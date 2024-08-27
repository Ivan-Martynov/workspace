#include "window_tools/WindowController.h"

#include <iostream>

namespace Marvin
{

bool WindowController::load_window(const int width, const int height,
    const char* title)
{
    // Initialize glfw.
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW.\n";
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
    m_window_ptr = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_window_ptr)
    {
        std::cerr << "Failed to create a GLFW window.\n";
        return false;
    }
    glfwMakeContextCurrent(m_window_ptr);

    // Load all OpenGL function pointers.
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD.\n";
        return false;
    }

    // Set the minimum number of screen update before buffers are swapped.
    glfwSwapInterval(1);

    return true;
}

/**
 * @brief Construct a new Window Loader:: Window Loader object.
 * 
 * @param width Window width.
 * @param height Window height.
 * @param title Window title.
 * 
 * @version 0.1
 * 
 * @date 2024-06-10
 */
WindowController::WindowController(const int width, const int height,
    const char* title): m_window_ptr {nullptr}
{
    if (!load_window(width, height, title))
    {
        m_window_ptr = nullptr;
    }
}

/**
 * @brief Construct a new Window Loader::Window Loader object with default
 * values.
 * 
 * @version 0.1
 * 
 * @date 2024-06-03
 */
WindowController::WindowController(): WindowController(400, 300, "")
{
}

WindowController::~WindowController()
{
    if (m_window_ptr)
    {
        glfwDestroyWindow(m_window_ptr);
    }
}

GLFWwindow* WindowController::get_pointer()
{
    return m_window_ptr;
}

bool WindowController::loaded() const
{
    return m_window_ptr != nullptr;
}

void WindowController::set_resize_callback(GLFWframebuffersizefun resize_callback)
{
    glfwSetFramebufferSizeCallback(m_window_ptr, resize_callback);
}

void WindowController::set_key_callback(GLFWkeyfun key_callback)
{
    glfwSetKeyCallback(m_window_ptr, key_callback);
}

} // namespace Marvin

