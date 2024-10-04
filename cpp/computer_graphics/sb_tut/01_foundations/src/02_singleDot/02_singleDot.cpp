
#include <OpenGL_base.h>
#include <Shader_helper.h>

namespace OpenGL_tutorial {

class Single_dot : public OpenGL_base
{
private:
    using OpenGL_base::OpenGL_base;

    GLuint program_ {};
    GLuint vao_ {};

    void virtual show_menu(const std::string&) override;
    void virtual process_input(const std::string&, const std::string&) override;
    void virtual game_loop(const int = 0) override;
};

void Single_dot::show_menu(const std::string& program_name) {
    std::cout << "Note: the program can be run as follows:\n" <<
        program_name << " int_param, where int_param is:\n" <<
        "0:\twindow with dots as pixels (default)\n";
    game_loop();
}

void Single_dot::process_input(const std::string& input,
        const std::string& program_name) {
    std::istringstream ss {input};
    int x;
    if (ss >> x) {
        switch (x) {
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

void Single_dot::game_loop(const int) {

    const std::string shaders_path {"../src/shaders/"};

    auto sources = std::map<GLenum, std::string>();
    sources[GL_VERTEX_SHADER] = shaders_path + "shader01.vert";
    sources[GL_FRAGMENT_SHADER] = shaders_path + "shader01.frag";

    program_ = Shader_helper::create_program(sources);

    glCreateVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    static constexpr GLfloat bg_color[] = {0.5, 0.7, 0.8, 1};

    glPointSize(25);

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        glClearBufferfv(GL_COLOR, 0, bg_color);

        glUseProgram(program_);

        glDrawArrays(GL_POINTS, 0, 3);

        glfwSwapBuffers(window_);
    }

    glDeleteVertexArrays(1, &vao_);
    glDeleteProgram(program_);

    glfwDestroyWindow(window_);
    glfwTerminate();
}

} // end namespace OpenGL_tutorial

int main(const int argc, const char *argv[]) {

    constexpr GLuint width {800}, height {600};
    OpenGL_tutorial::Single_dot game {width, height, "Single dot"};

    auto result = game.run(argc, argv);

    return result;
}

