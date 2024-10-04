
#include <algorithm>

#include <OpenGL_base.h>
#include <Shader_helper.h>
#include <Vertices_helper.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

namespace OpenGL_tutorial {

class Simple_plot2D : public OpenGL_base
{
private:
    using OpenGL_base::OpenGL_base;

    GLuint vao_ {};
    GLuint buffer_ {};
    GLuint program_ {};

    std::vector<glm::vec2> points_ {};

    static glm::mat4 projection_matrix_;

    void virtual show_menu(const std::string&) override;
    void virtual process_input(const std::string&, const std::string&) override;
    void virtual start_up() override;
    void virtual render(const int, const double = 0);
    void virtual shutdown() override;
    //void virtual game_loop(const int = 0) override;

    virtual void on_resize(const int, const int) override;

    void render_parameters();
    void load_shaders();
    void create_points();
    void resize_points();
    void load_vertex_array_objects();
    void load_buffers();
};

glm::mat4 Simple_plot2D::projection_matrix_ = glm::mat4{};

void Simple_plot2D::show_menu(const std::string& program_name) {
    std::cout << "Note: the program can be run as follows:\n" <<
        program_name << " int_param, where int_param is:\n" <<
        "0:\tDots (default)\n" <<
        "1:\tLines\n" <<
        "2:\tLines_strip\n" <<
        "3:\tLine_loop\n" <<
        "4:\tTriangle_strip\n" <<
        "5:\tTriangles\n";

    game_loop();
}

void Simple_plot2D::on_resize(const int w, const int h) {
    OpenGL_base::on_resize(w, h);
    projection_matrix_ = glm::ortho(0.0f, static_cast<float>(w),
            static_cast<float>(h), 0.0f, -1.0f, 1.0f);
    //resize_points();
}

void Simple_plot2D::process_input(const std::string& input,
        const std::string& program_name) {
    std::istringstream ss {input};
    int x;
    if (ss >> x) {
        game_loop(x);
    } else {
        std::cerr << "Wrong input: drawing default window\n";
        show_menu(program_name);
    }
}

void Simple_plot2D::render_parameters() {
    glPointSize(5);
    glfwSwapInterval(1);
}

void Simple_plot2D::load_vertex_array_objects() {
    glCreateVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
}

void Simple_plot2D::create_points() {
    constexpr auto two_pi = glm::pi<GLfloat>() * 2;

    constexpr int N = 100;
    constexpr GLfloat step = 1.0f / N;

    for (int i {-N}; i <= N; ++i) {
        const auto value = i * step;
        points_.emplace_back(value, std::sin(value * two_pi));
    }

    int win_w, win_h;
    glfwGetFramebufferSize(window_, &win_w, &win_h);

    int half_height = win_h >> 1;

    for (auto& p : points_) {
        p.x *= win_w;
        p.y *= half_height;
        p.y += half_height;
    }
}

void Simple_plot2D::resize_points() {
    int win_w, win_h;
    glfwGetFramebufferSize(window_, &win_w, &win_h);

    int half_height = win_h >> 1;

    for (auto& p : points_) {
        p.x *= win_w;
        p.y *= win_h;
        p.y += half_height;
    }
}

void Simple_plot2D::load_buffers() {
    glCreateBuffers(1, &buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_);

    glNamedBufferStorage(buffer_, Vertices_helper::size_of_elements(points_),
            points_.data(), GL_MAP_WRITE_BIT);

    auto ptr = reinterpret_cast<glm::vec2*>(glMapNamedBufferRange(buffer_, 0,
                Vertices_helper::size_of_elements(points_), GL_MAP_WRITE_BIT));

    std::copy(points_.begin(), points_.end(), ptr);

    glUnmapNamedBuffer(buffer_);

    /*
    glNamedBufferStorage(buffer_, Vertices_helper::size_of_elements(points_),
            points_.data(), GL_MAP_READ_BIT);
    //glNamedBufferStorage(buffer_, Vertices_helper::size_of_elements(points_),
    //        nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferSubData(buffer_, 0, Vertices_helper::size_of_elements(points_),
            points_.data());
    //glBufferData(GL_ARRAY_BUFFER, Vertices_helper::size_of_elements(points_),
    //        points_.data(), GL_STATIC_DRAW);
    */

    glVertexArrayVertexBuffer(vao_, 0, buffer_, 0, sizeof(glm::vec2));
    glVertexArrayAttribFormat(vao_, 0, 2, GL_FLOAT, GL_FALSE, 0);

    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(0);

    constexpr std::array<GLfloat, 4> color {0.9, 0.8, 0.1, 1};
    glVertexAttrib4fv(1, color.data());
}

void Simple_plot2D::load_shaders() {
    const std::string shaders_path {"../src/shaders/"};

    auto sources = std::map<GLenum, std::string>();
    sources[GL_VERTEX_SHADER] = shaders_path + "01_default.vert";
    //sources[GL_VERTEX_SHADER] = shaders_path + "01_coords.vert";
    sources[GL_FRAGMENT_SHADER] = shaders_path + "01_default.frag";

    program_ = Shader_helper::create_program(sources);
}

void Simple_plot2D::start_up() {
    render_parameters();

    load_vertex_array_objects();

    load_shaders();

    create_points();
    //resize_points();

    load_buffers();

    int win_w, win_h;
    glfwGetFramebufferSize(window_, &win_w, &win_h);

    projection_matrix_ = glm::ortho(0.0f, static_cast<float>(win_w),
            static_cast<float>(win_h), 0.0f, -1.0f, 1.0f);
}

void Simple_plot2D::render(const int option, const double) {
    static constexpr GLfloat bg_color[] = {0.5, 0.7, 0.8, 1};

    glClearBufferfv(GL_COLOR, 0, bg_color);

    const auto proj_location = glGetUniformLocation(program_,
            "projection_matrix");

    glUniformMatrix4fv(proj_location, 1, GL_FALSE,
            glm::value_ptr(projection_matrix_));

    glUseProgram(program_);

    switch (option) {
        case 0:
        default:
            glDrawArrays(GL_POINTS, 0, points_.size());
            break;
        case 1:
            glDrawArrays(GL_LINES, 0, points_.size());
            break;
        case 2:
            glDrawArrays(GL_LINE_STRIP, 0, points_.size());
            break;
        case 3:
            glDrawArrays(GL_LINE_LOOP, 0, points_.size());
            break;
        case 4:
            glDrawArrays(GL_TRIANGLE_STRIP, 0, points_.size());
            break;
        case 5:
            glDrawArrays(GL_TRIANGLES, 0, points_.size());
            break;
    };
}

void Simple_plot2D::shutdown() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &buffer_);
    glDeleteProgram(program_);
}

// function to compute sizeof elements lying in the vector container
template <class T>
constexpr size_t size_of_elements(const std::vector<T>& v) {
    return v.size() * sizeof(T);
}

//void Simple_plot2D::game_loop(const int option) {
//    OpenGL_base::game_loop(option);
//    static constexpr GLfloat bg_color[] = {0.5, 0.7, 0.8, 1};
//
//    glPointSize(5);
//    glfwSwapInterval(1);
//
//    while (!glfwWindowShouldClose(window_)) {
//        glfwPollEvents();
//        //glClearBufferfv(GL_COLOR, 0, bg_color);
//        glUseProgram(program_);
//
//        if (option == 1) {
//            glDrawArrays(GL_POINTS, 0, points_.size());
//        }
//        else {
//            //glDrawArrays(GL_LINE_STRIP, 0, points_.size());
//            glDrawArrays(GL_LINES, 0, points_.size());
//        }
//
//        glfwSwapBuffers(window_);
//    }
//
//    glfwDestroyWindow(window_);
//    glfwTerminate();
//}

} // end namespace OpenGL_tutorial

int main(const int argc, const char *argv[]) {

    using namespace OpenGL_tutorial;

    constexpr GLuint width {800}, height {600};

    auto game = std::make_unique<Simple_plot2D>(width, height, "Plot 2D");

    //std::unique_ptr<OpenGL_base> game = std::make_unique<Empty_window>(width,
    //height, "Empty window");

    return game->run(std::move(game), argc, argv);
}

