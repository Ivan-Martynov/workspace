
#include <OpenGL_base.h>
#include <Shader_helper.h>
#include <Texture_helper.h>
#include <Object_sb7.h>

#include <algorithm>
#include <experimental/filesystem>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OpenGL_tutorial {

class Object_with_texture : public OpenGL_base
{
private:
    using OpenGL_base::OpenGL_base;

    Object_sb7 object_ {};

    GLuint program_ {};
    std::vector<GLuint> textures_ {};
    static size_t texture_index_;

    static glm::mat4 proj_matrix_;

    static void resize_callback(GLFWwindow*, const int, const int);
    static void key_callback(GLFWwindow*, const int, const int, const int, const int);

    void virtual show_menu(const std::string&) override;
    void virtual process_input(const std::string&, const std::string&) override;
    void virtual game_loop(const int = 0) override;
};

glm::mat4 Object_with_texture::proj_matrix_ = glm::mat4{};
size_t Object_with_texture::texture_index_ = 0;

void Object_with_texture::show_menu(const std::string& program_name) {
    std::cout << "Note: the program can be run as follows:\n" <<
        program_name << " int_param, where int_param is:\n" <<
        "0:\ttorus with checker pattern texture " <<
        "(press 'R' to swap patterns)\n" <<
        "1:\ttorus with changing textures\n";
    game_loop();
}

void Object_with_texture::resize_callback(GLFWwindow* window, const int w,
        const int h) {
    OpenGL_base::resize_callback(window, w, h);
    proj_matrix_ = glm::perspective(glm::radians(60.0f),
            static_cast<GLfloat>(w) / h, 0.1f, 1000.0f);
}

void Object_with_texture::key_callback(GLFWwindow* window, const int key,
        const int scan, const int action, const int mod)
{
    OpenGL_base::key_callback(window, key, scan, action, mod);

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        texture_index_ = (texture_index_ == 0) ? 1 : 0;
}

void Object_with_texture::process_input(const std::string& input,
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

void Object_with_texture::game_loop(const int option) {
    glfwSetKeyCallback(window_, key_callback);

    namespace fs = std::experimental::filesystem;

    const std::string shaders_path {"../src/shaders/"};
    const std::string textures_path {"../bin/media/textures/"};
    const std::string objects_path {"../bin/media/objects/"};

    auto sources = std::map<GLenum, std::string>();
    sources[GL_VERTEX_SHADER] = shaders_path + "08_texture_coord.vert";
    sources[GL_FRAGMENT_SHADER] = shaders_path + "08_texture_coord.frag";

    if (option == 1) {
        for (const auto &p : fs::directory_iterator(textures_path))
            textures_.emplace_back(Texture_helper::load_texture(p.path()));
    } else {
        const std::string image_filename {textures_path + "pattern1.ktx"};
        textures_.emplace_back(Texture_helper::load_checker_pattern());
        textures_.emplace_back(Texture_helper::load_texture(image_filename));
    }

    glBindTexture(GL_TEXTURE_2D, textures_.front());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    object_.load(objects_path + "torus_nrms_tc.sbm");

    const size_t num_textures {textures_.size()};
    //const size_t num_textures {3};

    program_ = Shader_helper::create_program(sources);

    const auto mv_loc = glGetUniformLocation(program_, "modelview_matrix");
    const auto proj_loc = glGetUniformLocation(program_, "projection_matrix");

    int win_w, win_h;
    glfwGetFramebufferSize(window_, &win_w, &win_h);
    proj_matrix_ = glm::perspective(glm::radians(60.0f),
            static_cast<GLfloat>(win_w) / win_h, 0.1f, 1000.0f);

    static constexpr GLfloat bg_color[] = {0.5, 0.7, 0.8, 1};

    glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    constexpr GLfloat one_ {1.0f};

    size_t file_index {1};

    const GLfloat time_step {0.75f};
    //GLfloat time_iter_step {2.5f};
    const GLfloat time_threshold {num_textures * time_step};

    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        glClearBufferfv(GL_COLOR, 0, bg_color);
        glClearBufferfv(GL_DEPTH, 0, &one_);

        const GLfloat time {static_cast<GLfloat>(glfwGetTime())};

        glUseProgram(program_);

        glm::mat4 mv_matrix {};
        mv_matrix = glm::translate(mv_matrix, glm::vec3(0, 0, -3));
        mv_matrix = glm::rotate(mv_matrix, glm::radians(time * 19.3f),
                glm::vec3(0, 1, 0));
        mv_matrix = glm::rotate(mv_matrix, glm::radians(time * 21.1f),
                glm::vec3(0, 0, 1));

        glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_matrix));
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj_matrix_));

        if (option == 1) {
            const auto value = std::fmod(time, time_threshold);

            for (size_t i {0}; i < num_textures; ++i) {
                if (file_index < i)
                    break;

                if (value > i * time_step) {
                    if (file_index == num_textures) {
                        file_index = 1;
                        glBindTexture(GL_TEXTURE_2D, textures_.front());
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                GL_NEAREST);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                GL_NEAREST);
                        //time_iter_step = time_step;
                    }
                    else if (file_index == i) {
                        glBindTexture(GL_TEXTURE_2D, textures_[i]);
                        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        //        GL_LINEAR);
                        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        //        GL_LINEAR);
                        //glSamplerParameteri(GL_TEXTURE_2D,
                        //        GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        //glSamplerParameteri(GL_TEXTURE_2D,
                        //        GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        glTextureParameteri(textures_[i], GL_TEXTURE_BASE_LEVEL,
                                0);
                        glTextureParameteri(textures_[i], GL_TEXTURE_MAX_LEVEL,
                                16);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                GL_LINEAR_MIPMAP_NEAREST);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                GL_LINEAR);

                        ++file_index;
                        //time_iter_step += time_step;
                    }
                }
            }
        } else {
            glBindTexture(GL_TEXTURE_2D, textures_[texture_index_]);
        }

        object_.render();

        glfwSwapBuffers(window_);
    }

    glDeleteTextures(textures_.size(), textures_.data());
    glDeleteProgram(program_);

    glfwDestroyWindow(window_);
    glfwTerminate();
}

} // end namespace OpenGL_tutorial

int main(const int argc, const char *argv[]) {

    constexpr GLuint width {800}, height {600};
    OpenGL_tutorial::Object_with_texture game {width, height,
        "Object with texture"};

    auto result = game.run(argc, argv);

    return result;
}

