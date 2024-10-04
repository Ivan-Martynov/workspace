
#include <OpenGL_base.h>
#include <Shader_helper.h>
#include <Texture_helper.h>

#include <algorithm>
#include <experimental/filesystem>
#include <random>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OpenGL_tutorial {

class Alien_texture_rain : public OpenGL_base
{
private:
    using OpenGL_base::OpenGL_base;

    GLuint vao_ {};
    GLuint program_ {};

    GLuint texture_ {};
    GLuint buffer_ {};

    static constexpr size_t alien_num {256};

    std::array<float, alien_num> x_offset_ {};
    std::array<float, alien_num> rotation_speed_ {};
    std::array<float, alien_num> fall_speed_ {};

    static void key_callback(GLFWwindow*, const int, const int, const int, const
            int);

    void virtual show_menu(const std::string&) override;
    void virtual process_input(const std::string&, const std::string&) override;
    void virtual game_loop(const int = 0) override;
};


void Alien_texture_rain::show_menu(const std::string& program_name) {
    std::cout << "Note: the program can be run as follows:\n" <<
        //program_name << " int_param, where int_param is:\n" <<
        program_name << "\nDemonstrating texture arrays\n";
    game_loop();
}

void Alien_texture_rain::key_callback(GLFWwindow* window, const int key,
        const int scan, const int action, const int mod)
{
    OpenGL_base::key_callback(window, key, scan, action, mod);
}

void Alien_texture_rain::process_input(const std::string& input,
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

void Alien_texture_rain::game_loop(const int) {
    glfwSetKeyCallback(window_, key_callback);

    namespace fs = std::experimental::filesystem;

    const std::string shaders_path {"../src/shaders/"};
    const std::string textures_path {"../bin/media/textures/"};

    auto sources = std::map<GLenum, std::string>();
    sources[GL_VERTEX_SHADER] = shaders_path + "11_texture_coord.vert";
    sources[GL_FRAGMENT_SHADER] = shaders_path + "11_texture_coord.frag";

    program_ = Shader_helper::create_program(sources);
    texture_ = Texture_helper::load_texture(textures_path + "aliens.ktx");

    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_);
    glTextureParameteri(texture_, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);

    glCreateVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glCreateBuffers(1, &buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, buffer_);
    glBufferData(GL_UNIFORM_BUFFER, alien_num * sizeof(glm::vec4), nullptr,
            GL_DYNAMIC_DRAW);

    //std::random_device rd {};
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> dis {0, 1};

    for (size_t i {0}; i < alien_num; ++i) {
        x_offset_[i] = dis(gen) * 2 - 1;
        rotation_speed_[i] = (dis(gen) + 0.5f) * ((i & 1) ? -3 : 3);
        fall_speed_[i] = dis(gen) + 0.2f;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    static constexpr std::array<GLfloat, 4> bg_color {0.5, 0.7, 0.8, 1};

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        glClearBufferfv(GL_COLOR, 0, bg_color.data());

        glUseProgram(program_);

        const GLfloat time {static_cast<GLfloat>(glfwGetTime())};

        glBindBufferBase(GL_UNIFORM_BUFFER, 0, buffer_);
        auto droplet = static_cast<glm::vec4*>(glMapBufferRange(
                    GL_UNIFORM_BUFFER, 0, alien_num * sizeof(glm::vec4),
                    GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));

        for (size_t alien_index {0}; alien_index < alien_num; ++alien_index) {
            droplet[alien_index][0] = x_offset_[alien_index];
            droplet[alien_index][1] = 2 - fmodf((time + alien_index) *
                    fall_speed_[alien_index], 4.31f);
            droplet[alien_index][2] = time * rotation_speed_[alien_index];
            droplet[alien_index][3] = 0;
        //}

        //glUnmapBuffer(GL_UNIFORM_BUFFER);

        //for (size_t alien_index {0}; alien_index < alien_num; ++alien_index) {
            glVertexAttribI1i(0, alien_index);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        glUnmapBuffer(GL_UNIFORM_BUFFER);

        glfwSwapBuffers(window_);
    }

    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &buffer_);
    glDeleteTextures(1, &texture_);
    glDeleteProgram(program_);

    glfwDestroyWindow(window_);
    glfwTerminate();
}

} // end namespace OpenGL_tutorial

int main(const int argc, const char *argv[]) {

    constexpr GLuint width {800}, height {600};
    OpenGL_tutorial::Alien_texture_rain game {width, height, "Texture rain"};

    auto result = game.run(argc, argv);

    return result;
}

