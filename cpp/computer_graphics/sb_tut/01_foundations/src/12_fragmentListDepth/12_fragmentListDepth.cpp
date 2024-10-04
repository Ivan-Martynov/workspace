
#include <OpenGL_base.h>
#include <Shader_helper.h>
//#include <Texture_helper.h>
#include <Object_sb7.h>

#include <algorithm>
#include <experimental/filesystem>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OpenGL_tutorial {

class Fragment_list_depth : public OpenGL_base
{
private:
    using OpenGL_base::OpenGL_base;

    GLuint vao_ {};

    std::vector<GLuint> programs_ {};

    std::vector<GLuint> buffers_ {};

    GLuint head_pointer_image_ {};

    Object_sb7 object_ {};

    static glm::mat4 proj_matrix_;

    static void resize_callback(GLFWwindow*, const int, const int);
    static void key_callback(GLFWwindow*, const int, const int, const int, const
            int);

    void virtual show_menu(const std::string&) override;
    void virtual process_input(const std::string&, const std::string&) override;
    void virtual game_loop(const int = 0) override;
};

glm::mat4 Fragment_list_depth::proj_matrix_ = glm::mat4{};

void Fragment_list_depth::show_menu(const std::string& program_name) {
    std::cout << "Note: the program can be run as follows:\n" <<
        //program_name << " int_param, where int_param is:\n" <<
        program_name << "\nFragment list to demonstrate depth\n";
    game_loop();
}

void Fragment_list_depth::resize_callback(GLFWwindow* window, const int w,
        const int h) {
    OpenGL_base::resize_callback(window, w, h);
    proj_matrix_ = glm::perspective(glm::radians(60.0f),
            static_cast<GLfloat>(w) / h, 0.1f, 1000.0f);
}

void Fragment_list_depth::key_callback(GLFWwindow* window, const int key,
        const int scan, const int action, const int mod)
{
    OpenGL_base::key_callback(window, key, scan, action, mod);
}

void Fragment_list_depth::process_input(const std::string& input,
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

void Fragment_list_depth::game_loop(const int) {
    glfwSetKeyCallback(window_, key_callback);

    namespace fs = std::experimental::filesystem;

    const std::string shaders_path {"../src/shaders/"};
    //const std::string textures_path {"../bin/media/textures/"};
    const std::string objects_path {"../bin/media/objects/"};

    object_.load(objects_path + "dragon.sbm");

    auto sources = std::map<GLenum, std::string>();
    sources[GL_VERTEX_SHADER] = shaders_path + "12_clear.vert";
    sources[GL_FRAGMENT_SHADER] = shaders_path + "12_clear.frag";
    programs_.emplace_back(Shader_helper::create_program(sources));

    sources[GL_VERTEX_SHADER] = shaders_path + "12_append.vert";
    sources[GL_FRAGMENT_SHADER] = shaders_path + "12_append.frag";
    programs_.emplace_back(Shader_helper::create_program(sources));

    sources[GL_VERTEX_SHADER] = shaders_path + "12_resolve.vert";
    sources[GL_FRAGMENT_SHADER] = shaders_path + "12_resolve.frag";
    programs_.emplace_back(Shader_helper::create_program(sources));

    glCreateVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    buffers_ = std::vector<GLuint>(3);

    struct uniforms_block
    {
        glm::mat4 mv_matrix;
        glm::mat4 view_matrix;
        glm::mat4 proj_matrix;
    };

    glCreateBuffers(buffers_.size(), buffers_.data());

    glBindBuffer(GL_UNIFORM_BUFFER, buffers_[0]);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(uniforms_block), nullptr,
            GL_DYNAMIC_DRAW);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers_[1]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 1024 * 1024 * 16, nullptr,
            GL_DYNAMIC_COPY);

    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, buffers_[2]);
    glBufferData(GL_ATOMIC_COUNTER_BUFFER, 4, nullptr, GL_DYNAMIC_COPY);

    glCreateTextures(GL_TEXTURE_2D, 1, &head_pointer_image_);
    glBindTexture(GL_TEXTURE_2D, head_pointer_image_);
    glTextureStorage2D(head_pointer_image_, 1, GL_R32UI, 1024, 1024);

    int win_w, win_h;
    glfwGetFramebufferSize(window_, &win_w, &win_h);
    proj_matrix_ = glm::perspective(glm::radians(60.0f),
            static_cast<GLfloat>(win_w) / win_h, 0.1f, 1000.0f);

    static constexpr GLfloat bg_color[] = {0.5, 0.7, 0.8, 1};

    glfwSwapInterval(1);

    const auto mat_loc = glGetUniformLocation(programs_[1], "mvp");

    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        glClearBufferfv(GL_COLOR, 0, bg_color);

        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT |
                GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

        glUseProgram(programs_[0]);
        glBindVertexArray(vao_);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glUseProgram(programs_[1]);

        const auto time = static_cast<GLfloat>(glfwGetTime());

        glm::mat4 model_matrix {};
        model_matrix = glm::scale(model_matrix, glm::vec3(7.0f));

        glm::mat4 view_matrix = glm::lookAt(glm::vec3(
                    glm::cos(time * 0.35f) * 120,
                    glm::cos(time * 0.4f) * 30,
                    glm::sin(time * 0.35f) * 120),
                glm::vec3(0, 30, 0), glm::vec3(0, 1, 0));

        const auto mv_mat = view_matrix * model_matrix;
        glUniformMatrix4fv(mat_loc, 1, GL_FALSE,
                glm::value_ptr(proj_matrix_ * mv_mat));

        static constexpr unsigned int zero {0};
        glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, buffers_[2]);
        glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(zero), &zero);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buffers_[1]);

        glBindImageTexture(0, head_pointer_image_, 0, GL_FALSE, 0,
                GL_READ_WRITE, GL_R32UI);

        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT |
                GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

        object_.render();

        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT |
                GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

        glUseProgram(programs_[2]);

        glBindVertexArray(vao_);

        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT |
                GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window_);
    }

    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(buffers_.size(), buffers_.data());
    glDeleteTextures(1, &head_pointer_image_);
    for (auto x : programs_)
        glDeleteProgram(x);

    glfwDestroyWindow(window_);
    glfwTerminate();
}

} // end namespace OpenGL_tutorial

int main(const int argc, const char *argv[]) {

    constexpr GLuint width {800}, height {600};
    OpenGL_tutorial::Fragment_list_depth game {width, height,
        "Fragment list to demonstrate depth"};

    auto result = game.run(argc, argv);

    return result;
}

