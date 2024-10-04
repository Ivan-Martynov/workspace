
#include <OpenGL_base.h>
#include <Shader_helper.h>
//#include <Texture_helper.h>
//#include <Object_sb7.h>

#include <algorithm>
#include <experimental/filesystem>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OpenGL_tutorial {

class Shader_subroutines : public OpenGL_base
{
private:
    using OpenGL_base::OpenGL_base;

    GLuint vao_ {};

    GLuint program_ {};

    std::vector<GLuint> buffers_ {};

    GLuint head_pointer_image_ {};

    static glm::mat4 proj_matrix_;

    static void resize_callback(GLFWwindow*, const int, const int);

    void virtual show_menu(const std::string&) override;
    void virtual process_input(const std::string&, const std::string&) override;
    void virtual game_loop(const int = 0) override;
};

glm::mat4 Shader_subroutines::proj_matrix_ = glm::mat4{};

void Shader_subroutines::show_menu(const std::string& program_name) {
    std::cout << "Note: the program can be run as follows:\n" <<
        //program_name << " int_param, where int_param is:\n" <<
        program_name << "\nUsing shader subroutines to change color\n";
    game_loop();
}

void Shader_subroutines::resize_callback(GLFWwindow* window, const int w,
        const int h) {
    OpenGL_base::resize_callback(window, w, h);
    proj_matrix_ = glm::perspective(glm::radians(60.0f),
            static_cast<GLfloat>(w) / h, 0.1f, 1000.0f);
}

void Shader_subroutines::process_input(const std::string& input,
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

void Shader_subroutines::game_loop(const int) {
    glfwSetKeyCallback(window_, key_callback);

    namespace fs = std::experimental::filesystem;

    const std::string shaders_path {"../src/shaders/"};

    auto sources = std::map<GLenum, std::string>();
    sources[GL_VERTEX_SHADER] = shaders_path + "13_subroutine.vert";
    sources[GL_FRAGMENT_SHADER] = shaders_path + "13_subroutine.frag";
    program_ = Shader_helper::create_program(sources);

    //const GLuint subroutine_loc = glGetSubroutineUniformLocation(program_,
    //        GL_FRAGMENT_SHADER, "sub_changeColor");

    const GLuint subroutine1 = glGetSubroutineIndex(program_,
            GL_FRAGMENT_SHADER, "changeColor1");
    const GLuint subroutine2 = glGetSubroutineIndex(program_,
            GL_FRAGMENT_SHADER, "changeColor2");

    glCreateVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    static constexpr GLfloat bg_color[] = {0.5, 0.7, 0.8, 1};

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        glClearBufferfv(GL_COLOR, 0, bg_color);

        glUseProgram(program_);

        const auto time = static_cast<int>(glfwGetTime());

        if (time & 1)
            glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &subroutine1);
        else
            glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &subroutine2);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window_);
    }

    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(buffers_.size(), buffers_.data());
    glDeleteProgram(program_);

    glfwDestroyWindow(window_);
    glfwTerminate();
}

} // end namespace OpenGL_tutorial

int main(const int argc, const char *argv[]) {

    constexpr GLuint width {800}, height {600};
    OpenGL_tutorial::Shader_subroutines game {width, height,
        "Demonstration of shader subroutines"};

    auto result = game.run(argc, argv);

    return result;
}

