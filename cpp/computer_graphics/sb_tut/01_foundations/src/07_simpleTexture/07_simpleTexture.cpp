
#include <OpenGL_base.h>
#include <Shader_helper.h>
#include <Texture_helper.h>
#include <experimental/filesystem>

namespace OpenGL_tutorial {

class Simple_texture : public OpenGL_base
{
private:
    using OpenGL_base::OpenGL_base;

    GLuint program_ {};
    GLuint vao_ {};
    GLuint texture_ {};

    void virtual show_menu(const std::string&) override;
    void virtual process_input(const std::string&, const std::string&) override;
    void virtual game_loop(const int = 0) override;
};

void Simple_texture::show_menu(const std::string& program_name) {
    std::cout << "Note: the program can be run as follows:\n" <<
        program_name << " int_param, where int_param is:\n" <<
        "0:\tsimple pattern texture (default)\n" <<
        "1:\ttextures changing every second\n";
    game_loop();
}

void Simple_texture::process_input(const std::string& input,
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

void Simple_texture::game_loop(const int option) {
    namespace fs = std::experimental::filesystem;

    const std::string shaders_path {"../src/shaders/"};
    const std::string textures_path {"../bin/media/textures/"};

    auto sources = std::map<GLenum, std::string>();
    sources[GL_VERTEX_SHADER] = shaders_path + "shader01.vert";
    sources[GL_FRAGMENT_SHADER] = shaders_path + "shader_texture2D_01.frag";

    //const std::string image_filename {textures_path + "floor.ktx"};

    std::vector<std::string> file_names {};

    size_t file_index {0};
    size_t num_files {};

    if (option == 1) {
        for (const auto &p : fs::directory_iterator(textures_path))
            file_names.emplace_back(p.path());
        num_files = file_names.size();

        texture_ = Texture_helper::load_texture(file_names[file_index],
                texture_);
    } else {
        num_files = 0;
        texture_ =  Texture_helper::load_default_texture(texture_);
    }

    program_ = Shader_helper::create_program(sources);

    glCreateVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    static constexpr GLfloat bg_color[] = {0.5, 0.7, 0.8, 1};

    glfwSwapInterval(1);

    bool update_texture {true};
    GLfloat prev_value {0};

    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        glClearBufferfv(GL_COLOR, 0, bg_color);

        glUseProgram(program_);

        if (option == 1) {
            const auto value = std::fmod(glfwGetTime(), num_files);
            if ((value > file_index + 1)) {
                if (update_texture) {
                    update_texture = false;
                    texture_ =
                        Texture_helper::load_texture(file_names[file_index],
                                texture_);
                    if (++file_index >= num_files)
                        file_index = 0;
                }
            } else {
                update_texture = true;
            }
            if (prev_value > value)
                file_index = 0;
            prev_value = value;
        }

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window_);
    }

    glDeleteVertexArrays(1, &vao_);
    glDeleteTextures(1, &texture_);
    glDeleteProgram(program_);

    glfwDestroyWindow(window_);
    glfwTerminate();
}

} // end namespace OpenGL_tutorial

int main(const int argc, const char *argv[]) {

    constexpr GLuint width {800}, height {600};
    OpenGL_tutorial::Simple_texture game {width, height, "Simple texture"};

    auto result = game.run(argc, argv);

    return result;
}

