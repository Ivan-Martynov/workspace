
#include <OpenGL_base.h>
#include <Shader_helper.h>
#include <Texture_helper.h>

#include <algorithm>
#include <experimental/filesystem>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OpenGL_tutorial {

class Wrap_modes_example : public OpenGL_base
{
private:
    using OpenGL_base::OpenGL_base;

    GLuint vao_ {};
    GLuint program_ {};
    std::vector<GLuint> textures_ {};

    static void key_callback(GLFWwindow*, const int, const int, const int, const
            int);

    void virtual show_menu(const std::string&) override;
    void virtual process_input(const std::string&, const std::string&) override;
    void virtual game_loop(const int = 0) override;
};


void Wrap_modes_example::show_menu(const std::string& program_name) {
    std::cout << "Note: the program can be run as follows:\n" <<
        //program_name << " int_param, where int_param is:\n" <<
        program_name << "\nDemonstrating different wrap modes\n";
    game_loop();
}

void Wrap_modes_example::key_callback(GLFWwindow* window, const int key,
        const int scan, const int action, const int mod)
{
    OpenGL_base::key_callback(window, key, scan, action, mod);
}

void Wrap_modes_example::process_input(const std::string& input,
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

void Wrap_modes_example::game_loop(const int) {
    glfwSetKeyCallback(window_, key_callback);

    namespace fs = std::experimental::filesystem;

    const std::string shaders_path {"../src/shaders/"};
    const std::string textures_path {"../bin/media/textures/"};

    auto sources = std::map<GLenum, std::string>();
    sources[GL_VERTEX_SHADER] = shaders_path + "10_texture_coord.vert";
    sources[GL_FRAGMENT_SHADER] = shaders_path + "10_texture_coord.frag";

    const GLuint texture_arrows = Texture_helper::load_texture(textures_path +
            "rightarrows.ktx");

    textures_.emplace_back(texture_arrows);

    glBindTexture(GL_TEXTURE_2D, textures_.front());

    program_ = Shader_helper::create_program(sources);

    glCreateVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    static constexpr std::array<GLfloat, 4> bg_color {0.5, 0.7, 0.8, 1};
    static constexpr std::array<GLfloat, 4> border_color {0.2, 0.5, 0.1, 1};

    static constexpr size_t modes_num {4};

    static constexpr std::array<GLenum, modes_num> wrap_modes {
        GL_CLAMP_TO_EDGE, GL_REPEAT, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT
    };

    static constexpr std::array<float, modes_num << 1> offsets {
        -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f
    };

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        glClearBufferfv(GL_COLOR, 0, bg_color.data());

        glUseProgram(program_);

        for (size_t i {0}; i < modes_num; ++i) {
            if (i == 0)
                glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,
                        border_color.data());
            glUniform2fv(0, 1, &offsets[i << 1]);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_modes[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_modes[i]);

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
    OpenGL_tutorial::Wrap_modes_example game {width, height,
        "Wrapping examples"};

    auto result = game.run(argc, argv);

    return result;
}

