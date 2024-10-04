
#include <OpenGL_base.h>
#include <Shader_helper.h>
#include <Texture_helper.h>
#include <Object_sb7.h>

#include <algorithm>
#include <experimental/filesystem>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OpenGL_tutorial {

class Tunnel_mipmap : public OpenGL_base
{
private:
    using OpenGL_base::OpenGL_base;

    GLuint vao_ {};
    GLuint program_ {};
    std::vector<GLuint> textures_ {};

    static glm::mat4 proj_matrix_;

    static void resize_callback(GLFWwindow*, const int, const int);
    static void key_callback(GLFWwindow*, const int, const int, const int, const
            int);

    void virtual show_menu(const std::string&) override;
    void virtual process_input(const std::string&, const std::string&) override;
    void virtual game_loop(const int = 0) override;
};

glm::mat4 Tunnel_mipmap::proj_matrix_ = glm::mat4{};

void Tunnel_mipmap::show_menu(const std::string& program_name) {
    std::cout << "Note: the program can be run as follows:\n" <<
        //program_name << " int_param, where int_param is:\n" <<
        program_name << "\ncorridor demonstrating the use of mipmaps\n";
    game_loop();
}

void Tunnel_mipmap::resize_callback(GLFWwindow* window, const int w,
        const int h) {
    OpenGL_base::resize_callback(window, w, h);
    proj_matrix_ = glm::perspective(glm::radians(60.0f),
            static_cast<GLfloat>(w) / h, 0.1f, 1000.0f);
}

void Tunnel_mipmap::key_callback(GLFWwindow* window, const int key,
        const int scan, const int action, const int mod)
{
    OpenGL_base::key_callback(window, key, scan, action, mod);
}

void Tunnel_mipmap::process_input(const std::string& input,
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

void Tunnel_mipmap::game_loop(const int) {
    glfwSetKeyCallback(window_, key_callback);

    namespace fs = std::experimental::filesystem;

    const std::string shaders_path {"../src/shaders/"};
    const std::string textures_path {"../bin/media/textures/"};

    auto sources = std::map<GLenum, std::string>();
    sources[GL_VERTEX_SHADER] = shaders_path + "09_texture_coord.vert";
    sources[GL_FRAGMENT_SHADER] = shaders_path + "09_texture_coord.frag";

    glCreateVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    const GLuint texture_wall = Texture_helper::load_texture(textures_path +
            "brick.ktx");
    const GLuint texture_ceiling = Texture_helper::load_texture(textures_path +
            "ceiling.ktx");
    const GLuint texture_floor = Texture_helper::load_texture(textures_path +
            "floor.ktx");

    textures_.emplace_back(texture_floor);
    textures_.emplace_back(texture_wall);
    textures_.emplace_back(texture_ceiling);

    for (const auto tex : textures_) {
        glBindTexture(GL_TEXTURE_2D, tex);
        glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    textures_.insert(textures_.begin(), texture_wall);

    const size_t num_textures {textures_.size()};

    //glBindVertexArray(vao_);

    program_ = Shader_helper::create_program(sources);

    const auto matrix_loc = glGetUniformLocation(program_,
            "transformation_matrix");
    const auto offset_loc = glGetUniformLocation(program_, "offset");

    int win_w, win_h;
    glfwGetFramebufferSize(window_, &win_w, &win_h);
    proj_matrix_ = glm::perspective(glm::radians(60.0f),
            static_cast<GLfloat>(win_w) / win_h, 0.1f, 1000.0f);

    static constexpr GLfloat bg_color[] = {0.5, 0.7, 0.8, 1};

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        glClearBufferfv(GL_COLOR, 0, bg_color);

        const GLfloat time {static_cast<GLfloat>(glfwGetTime())};

        glUseProgram(program_);

        glUniform1f(offset_loc, time * 0.003f);

        for (size_t i {0}; i < num_textures; ++i) {

            glm::mat4 mv_matrix {};
            mv_matrix = glm::rotate(mv_matrix, glm::radians(90.0f * i),
                    glm::vec3(0, 0, 1));
            mv_matrix = glm::translate(mv_matrix, glm::vec3(-0.5f, 0, -10));
            mv_matrix = glm::rotate(mv_matrix, glm::radians(90.0f),
                    glm::vec3(0, 1, 0));
            mv_matrix = glm::scale(mv_matrix, glm::vec3(30, 1, 1));

            glUniformMatrix4fv(matrix_loc, 1, GL_FALSE,
                    glm::value_ptr(proj_matrix_ * mv_matrix));

            glBindTexture(GL_TEXTURE_2D, textures_[i]);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        glfwSwapBuffers(window_);
    }

    glDeleteVertexArrays(1, &vao_);
    glDeleteTextures(textures_.size(), textures_.data());
    glDeleteProgram(program_);

    glfwDestroyWindow(window_);
    glfwTerminate();
}

} // end namespace OpenGL_tutorial

int main(const int argc, const char *argv[]) {

    constexpr GLuint width {800}, height {600};
    OpenGL_tutorial::Tunnel_mipmap game {width, height, "Tunnel"};

    auto result = game.run(argc, argv);

    return result;
}

