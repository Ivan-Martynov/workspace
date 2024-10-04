
#include <OpenGL_base.h>
#include <Shader_helper.h>

namespace OpenGL_tutorial {

class Simple_triangle : public OpenGL_base
{
private:
    using OpenGL_base::OpenGL_base;

    GLuint program_ {};
    GLuint vao_ {};

    void virtual show_menu(const std::string&) override;
    void virtual process_input(const std::string&, const std::string&) override;
    void virtual game_loop(const int = 0) override;
};

void Simple_triangle::show_menu(const std::string& program_name) {
    std::cout << "Note: the program can be run as follows:\n" <<
        program_name << " int_param, where int_param is:\n" <<
        "0:\ttriangle (default)\n" <<
        "1:\tmoving triangle\n";
    game_loop();
}

void Simple_triangle::process_input(const std::string& input,
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

void Simple_triangle::game_loop(const int option) {

    const std::string shaders_path {"../src/shaders/"};

    auto sources = std::map<GLenum, std::string>();
    sources[GL_VERTEX_SHADER] = shaders_path + "shader_triangle.vert";
    sources[GL_FRAGMENT_SHADER] = shaders_path + "shader_triangle.frag";

    program_ = Shader_helper::create_program(sources);

    glCreateVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    static constexpr GLfloat bg_color[] = {0.5, 0.7, 0.8, 1};

    static constexpr GLfloat triangle_color[] = {0.3, 0.4, 0.2, 1};

    glVertexAttrib4fv(1, triangle_color);

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        glClearBufferfv(GL_COLOR, 0, bg_color);

        glUseProgram(program_);

        GLfloat offset[] = {0, 0, 0, 0};

        if (option == 1) {
            const auto time = glfwGetTime();

            offset[0] = glm::sin(time) * 0.5f;
            offset[1] = glm::cos(time) * 0.5f;
        }

        glVertexAttrib4fv(0, offset);

        glDrawArrays(GL_TRIANGLES, 0, 3);

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
    OpenGL_tutorial::Simple_triangle game {width, height, "First triangle"};

    auto result = game.run(argc, argv);

    return result;
}

