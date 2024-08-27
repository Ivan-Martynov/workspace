#ifndef _H_WINDOW_CONTROLLER_H_
#define _H_WINDOW_CONTROLLER_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Marvin
{

class WindowController
{
  private:
    GLFWwindow* m_window_ptr;

    bool load_window(const int, const int, const char*);

    enum class load_codes {
        SUCCESS,
        FAIL_GLFW_INIT,
        FAIL_GLFW_WINDOW_CREATE,
        FAIL_GLAD_INIT,
    };

  public:
    explicit WindowController();
    explicit WindowController(const int, const int, const char*);

    explicit WindowController(const WindowController&) = delete;
    explicit WindowController(WindowController&&) = delete;
    WindowController& operator=(const WindowController&) = delete;
    WindowController& operator=(WindowController&&) = delete;

    ~WindowController();

    GLFWwindow* get_pointer();

    bool loaded() const;

    void set_resize_callback(GLFWframebuffersizefun);
    void set_key_callback(GLFWkeyfun);
};

} // namespace Marvin

#endif //_H_WINDOW_CONTROLLER_H_
