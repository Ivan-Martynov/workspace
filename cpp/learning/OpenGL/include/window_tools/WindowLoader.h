#ifndef _H_WINDOWLOADER_H_
#define _H_WINDOWLOADER_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Marvin
{

class WindowLoader
{
  private:
    GLFWwindow* m_window_ptr;

    bool load_window();

    enum class load_codes {
        SUCCESS,
        FAIL_GLFW_INIT,
        FAIL_GLFW_WINDOW_CREATE,
        FAIL_GLAD_INIT,
    };

  public:
    explicit WindowLoader();

    GLFWwindow* get_pointer();

    bool loaded() const;

    void set_resize_callback(GLFWframebuffersizefun);
    void set_key_callback(GLFWkeyfun);
};

} // namespace Marvin

#endif //_H_WINDOWLOADER_H_
