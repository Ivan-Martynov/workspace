
#include <OpenGL_base.h>

namespace OpenGL_tutorial {

class Empty_window : public OpenGL_base
{
private:
    using OpenGL_base::OpenGL_base;

    void virtual show_menu(const std::string&) override;
    void virtual process_input(const std::string&, const std::string&) override;
    void virtual game_loop(const int = 0) override;
};

void Empty_window::show_menu(const std::string& program_name) {
    std::cout << "Note: the program can be run as follows:\n" <<
        program_name << " int_param, where int_param is:\n" <<
        "0:\tempty window (default)\n" <<
        "1:\twindow with changing color\n";
    game_loop();
}

void Empty_window::process_input(const std::string& input,
        const std::string& program_name) {
    std::istringstream ss {input};
    int x;
    if (ss >> x) {
        switch (x) {
            case 1:
                game_loop(1);
                break;
            case 0:
                game_loop(0);
                break;
            default:
                goto wrong_input;
        }
        return;
    }

wrong_input:
    std::cerr << "Wrong input: drawing default window\n";
    show_menu(program_name);
}

void Empty_window::game_loop(const int option) {

    static constexpr GLfloat bg_color[] = {0.5, 0.7, 0.8, 1};

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        if (option == 1) {
            const auto time = glfwGetTime();
            const GLfloat custom_color[] = {
                static_cast<GLfloat>(0.5 * (glm::sin(time) + 1)),
                static_cast<GLfloat>(0.5 * (glm::cos(time) + 1)), 0.0f, 1.0f};
            glClearBufferfv(GL_COLOR, 0, custom_color);
        } else {
            glClearBufferfv(GL_COLOR, 0, bg_color);
        }

        glfwSwapBuffers(window_);
    }

    glfwDestroyWindow(window_);
    glfwTerminate();
}

} // end namespace OpenGL_tutorial

int main(const int argc, const char *argv[]) {

    constexpr GLuint width {800}, height {600};

    OpenGL_tutorial::Empty_window game {width, height, "Empty window"};

    return game.run(argc, argv);
}

