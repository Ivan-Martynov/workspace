
#include <OpenGL_base.h>
#include <Shader_helper.h>
#include <Vertices_helper.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OpenGL_tutorial {

class Spinning_cubes : public OpenGL_base
{
private:
    using OpenGL_base::OpenGL_base;

    GLuint vao_ {};
    GLuint buffer_ {};
    GLuint program_ {};

    static glm::mat4 projection_matrix_;

    static void resize_callback(GLFWwindow*, const int, const int);

    void virtual show_menu(const std::string&) override;
    void virtual process_input(const std::string&, const std::string&) override;
    void virtual game_loop(const int = 0) override;
};

glm::mat4 Spinning_cubes::projection_matrix_ = glm::mat4{};

void Spinning_cubes::show_menu(const std::string& program_name) {
    std::cout << "Note: the program can be run as follows:\n" <<
        program_name << " int_param, where int_param is:\n" <<
        "0:\tsingle cube (default)\n" <<
        "1:\tmany cubes\n";
    game_loop();
}

void Spinning_cubes::resize_callback(GLFWwindow* window, const int w,
        const int h) {
    OpenGL_base::resize_callback(window, w, h);
    projection_matrix_ = glm::perspective(glm::radians(60.0f),
            static_cast<GLfloat>(w) / h, 0.1f, 1000.0f);
}

void Spinning_cubes::process_input(const std::string& input,
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

void Spinning_cubes::game_loop(const int option) {

    //glfwSetWindowRefreshCallback(window_, window_refresh_callback);
    glfwSetWindowSizeCallback(window_, resize_callback);

    //glMemoryBarrier(GL_ALL_BARRIER_BITS);

    const std::string shaders_path {"../src/shaders/"};

    auto sources = std::map<GLenum, std::string>();
    sources[GL_VERTEX_SHADER] = shaders_path + "shader_cube_01.vert";
    sources[GL_FRAGMENT_SHADER] = shaders_path + "shader_triangle.frag";

    program_ = Shader_helper::create_program(sources);

    glCreateVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    static constexpr GLfloat bg_color[] = {0.5, 0.7, 0.8, 1};

    static constexpr auto vertices = Vertices_helper::cube_vertices();

    glCreateBuffers(1, &buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
            GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    int win_w, win_h;
    glfwGetFramebufferSize(window_, &win_w, &win_h);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glfwSwapInterval(1);

    static constexpr GLfloat one_ {1.0f};

    const auto mv_location = glGetUniformLocation(program_,
            "modelview_matrix");
    const auto proj_location = glGetUniformLocation(program_,
            "projection_matrix");

    projection_matrix_ = glm::perspective(glm::radians(60.0f),
            static_cast<GLfloat>(win_w) / win_h, 0.1f, 1000.0f);

    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        glClearBufferfv(GL_COLOR, 0, bg_color);
        glClearBufferfv(GL_DEPTH, 0, &one_);
        //glClearBufferfi(GL_DEPTH_STENCIL, 0, 1, 0);

        glUniformMatrix4fv(proj_location, 1, GL_FALSE,
                glm::value_ptr(projection_matrix_));

        glUseProgram(program_);

        const GLfloat time {static_cast<GLfloat>(glfwGetTime())};

        if (option == 1) {
            for (int i = 0; i < 24; ++i) {
                const auto f = i + time * glm::pi<GLfloat>() * 0.3f;

                glm::mat4 trans {};
                trans = glm::translate(trans, glm::vec3(0.5, -0.5, -10));
                trans = glm::rotate(trans, glm::radians(time * 45),
                        glm::vec3(0, 1, 0));
                trans = glm::rotate(trans, glm::radians(time * 21),
                        glm::vec3(1, 0, 0));

                trans = glm::translate(trans, glm::vec3(
                            glm::sin(2.1 * f) * 2.0f,
                            glm::cos(1.7 * f) * 2.0f,
                            glm::sin(1.3 * f)* glm::cos(1.5 * f) * 2.0f));

                glUniformMatrix4fv(mv_location, 1, GL_FALSE,
                        glm::value_ptr(trans));

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        } else {
            const auto f = time * glm::pi<GLfloat>() * 0.1f;

            glm::mat4 trans {};
            trans = glm::translate(trans, glm::vec3(0.5, -0.5, -4));
            trans = glm::translate(trans, glm::vec3(
                        glm::sin(2.1 * f) * 0.5f,
                        glm::cos(1.7 * f) * 0.5f,
                        glm::sin(1.3 * f)* glm::cos(1.5 * f) * 2.0f));
            trans = glm::rotate(trans, glm::radians(time * 45),
                    glm::vec3(0, 1, 0));
            trans = glm::rotate(trans, glm::radians(time * 81),
                    glm::vec3(1, 0, 0));

            glUniformMatrix4fv(mv_location, 1, GL_FALSE, glm::value_ptr(trans));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window_);
    }

    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &buffer_);
    glDeleteProgram(program_);

    glfwDestroyWindow(window_);
    glfwTerminate();
}

} // end namespace OpenGL_tutorial

int main(const int argc, const char *argv[]) {

    constexpr GLuint width {800}, height {600};
    OpenGL_tutorial::Spinning_cubes game {width, height, "Spinning cubes"};

    auto result = game.run(argc, argv);

    return result;
}

