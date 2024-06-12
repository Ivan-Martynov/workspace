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

void key_callback(GLFWwindow* window_ptr, const int key, const int,
        const int action, const int)
{
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS))
    {
        glfwSetWindowShouldClose(window_ptr, GLFW_TRUE);
    }
}

// Process inputs.
void process_input(GLFWwindow* window_ptr)
{
    if (glfwGetKey(window_ptr, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window_ptr, true);
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
//    glfwSetFramebufferSizeCallback(window_loader.window_ptr,
 //       framebuffer_size_callback);

    // Load all OpenGL function pointers.
    //if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    //{
    //    std::cout << "Failed to initialize GLAD.\n";
    //    window_loader.state = load_codes::FAIL_GLAD_INIT;
    //    return false;
    //}

    window_loader.state = load_codes::SUCCESS;

    //glfwSetKeyCallback(window_loader.window_ptr, key_callback);

    //glfwSwapInterval(1);

    return true;
}

//void update_window(GLFWwindow* window_ptr)
//{
    //process_input(window_ptr);
//}

void render_window(GLFWwindow*)
{
    static constexpr GLfloat bg_color[] = {0.2f, 0.3f, 0.3f, 1.0f};

    glClearBufferfv(GL_COLOR, 0, bg_color);
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT);
}

int main()
{
    struct WindowLoader window_loader {};
    if (!load_window(window_loader))
    {
        return static_cast<int>(window_loader.state);
    }
    
    while(!glfwWindowShouldClose(window_loader.window_ptr))
    {
        glfwPollEvents();    

        //update_window(window_loader.window_ptr);
        //render_window(window_loader.window_ptr);

        glfwSwapBuffers(window_loader.window_ptr);
    }

    glfwDestroyWindow(window_loader.window_ptr);
    glfwTerminate();

    return 0;
}
