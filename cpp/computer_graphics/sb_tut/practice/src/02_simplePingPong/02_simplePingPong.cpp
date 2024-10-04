
#include <algorithm>

#include <OpenGL_base.h>
#include <Shader_helper.h>
#include <Vertices_helper.h>
#include <PingPongPad.h>
#include <Ball2D.h>
#include <Renderer_2D.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#include <AL/al.h>
#include <AL/alc.h>

#include <audio/wave.h>

namespace OpenGL_tutorial {

enum class Game_state { active, paused, win };

class Simple_PingPong : public OpenGL_base
{
private:
    using OpenGL_base::OpenGL_base;

    glm::vec2 window_size_ {};

    GLuint program_ {};

    std::array<GLboolean, 1024> keys_ {};

    PingPongPad left_pad_ {};
    PingPongPad right_pad_ {};
    Ball_2D ball_ {};

    Game_state game_state_ {Game_state::active};

    Renderer_2D renderer {};

    static glm::mat4 projection_matrix_;

    void virtual show_menu(const std::string&) override;
    void virtual process_input(const std::string&, const std::string&) override;
    void virtual start_up() override;
    void virtual render(const int, const double = 0) override;
    void virtual shutdown() override;
    void virtual game_loop(const int = 0) override;

    void init_audio();
    void calculate_window_size();

    virtual void on_resize(const int, const int) override;
    virtual void on_key(const int, const int, const int, const int) override;

    constexpr std::array<GLfloat, 8> quad_vertices();
    constexpr std::array<GLfloat, 8> quad_vertices(const int, const int);
    constexpr std::array<GLuint, 6> quad_indices();

    void set_objects();

    void load_shaders();

    void move_ball();
};

glm::mat4 Simple_PingPong::projection_matrix_ = glm::mat4{};

void Simple_PingPong::show_menu(const std::string&) {
    std::cout << "Simple ping pong game\n";

    game_loop();
}

void Simple_PingPong::on_resize(const int w, const int h) {
    OpenGL_base::on_resize(w, h);

    projection_matrix_ = glm::ortho(0.0f, static_cast<float>(w),
            static_cast<float>(h), 0.0f, -1.0f, 1.0f);

    GLfloat aspect_x = w / window_size_.x;
    GLfloat aspect_y = h / window_size_.y;

    std::vector<std::reference_wrapper<Game_object_2D>> objects {};
    objects.push_back(left_pad_);
    objects.push_back(right_pad_);
    objects.push_back(ball_);

    for (auto x : objects)
        x.get().scale(aspect_x, aspect_y);

    window_size_.x = w;
    window_size_.y = h;

    if (window_size_.x > window_size_.y)
        ball_.set_size(window_size_.y * 0.03f);
    else
        ball_.set_size(window_size_.x * 0.03f);

    std::cout << "Ball size:\t" << ball_.size().x << '\t' << ball_.size().y << '\n';
    std::cout << "Pad size:\t" << left_pad_.size().x << '\t' << left_pad_.size().y << '\n';
}

void Simple_PingPong::process_input(const std::string& input,
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

void Simple_PingPong::set_objects() {
    const GLfloat pad_width = 0.1f * window_size_.y;
    const GLfloat thickness = 0.01f * window_size_.x;

    left_pad_.set_position(thickness * 0.5, window_size_.y * 0.5f);
    left_pad_.set_size(thickness, pad_width);
    left_pad_.set_color(1, 1, 1, 1);

    right_pad_.set_position(window_size_.x - thickness * 0.5, window_size_.y * 0.5f);
    right_pad_.set_size(thickness, pad_width);
    right_pad_.set_color(1, 1, 1, 1);

    if (window_size_.x > window_size_.y)
        ball_.set_size(window_size_.y * 0.03f);
    else
        ball_.set_size(window_size_.x * 0.03f);

    ball_.set_position(window_size_.x * 0.5f, window_size_.y * 0.5f);
    ball_.set_color(1, 1, 1, 1);
    ball_.set_velocity(3, 3);
}

void Simple_PingPong::load_shaders() {
    const std::string shaders_path {"../src/shaders/"};

    auto sources = std::map<GLenum, std::string>();
    sources[GL_VERTEX_SHADER] = shaders_path + "pong_pad.vert";
    sources[GL_FRAGMENT_SHADER] = shaders_path + "01_default.frag";

    program_ = Shader_helper::create_program(sources);

    renderer.init(program_);
}

void Simple_PingPong::calculate_window_size() {
    int win_w, win_h;
    glfwGetFramebufferSize(window_, &win_w, &win_h);

    projection_matrix_ = glm::ortho(0.0f, static_cast<float>(win_w),
            static_cast<float>(win_h), 0.0f, -1.0f, 1.0f);

    window_size_.x = win_w;
    window_size_.y = win_h;
}

void Simple_PingPong::start_up() {
    glfwSwapInterval(1);

    calculate_window_size();

    load_shaders();

    set_objects();
}

void Simple_PingPong::move_ball() {
    auto x = ball_.position().x;
    auto y = ball_.position().y;

    auto ball_x = x + ball_.velocity().x;
    auto ball_y = y + ball_.velocity().y;

    if (ball_y >= window_size_.y) {
        ball_y = ball_.position().y - ball_.velocity().y;
        ball_.set_velocity_y(-ball_.velocity().y);
    } else if (ball_y <= 0) {
        ball_y = ball_.position().y - ball_.velocity().y;
        ball_.set_velocity_y(-ball_.velocity().y);
    }

    if (ball_x + ball_.radius() >= window_size_.x - right_pad_.size().x) {
        if (y >= right_pad_.position().y - right_pad_.size().y * 0.5f &&
                y <= right_pad_.position().y + right_pad_.size().y * 0.5f) {
            ball_x = x - ball_.velocity().x;
            ball_.set_velocity_x(-ball_.velocity().x);
        } else {
            ball_x = window_size_.x * 0.5f;
            ball_y = window_size_.y * 0.5f;
        }
    } else if (ball_x - ball_.radius() <= left_pad_.size().x) {
        if (y >= left_pad_.position().y - left_pad_.size().y * 0.5f &&
                y <= left_pad_.position().y + left_pad_.size().y * 0.5f) {
            ball_x = x - ball_.velocity().x;
            ball_.set_velocity_x(-ball_.velocity().x);
        } else {
            ball_x = window_size_.x * 0.5f;
            ball_y = window_size_.y * 0.5f;
        }
    }

    ball_.set_position(ball_x, ball_y);
}

void Simple_PingPong::render(const int, const double) {
    const auto proj_location = glGetUniformLocation(program_,
            "projection_matrix");

    glUniformMatrix4fv(proj_location, 1, GL_FALSE,
            glm::value_ptr(projection_matrix_));

    constexpr GLfloat velocity {5};

    move_ball();

    if (keys_[GLFW_KEY_W]) {
        const GLfloat d = left_pad_.position().y - velocity;
        left_pad_.set_position_y(d > left_pad_.size().y * 0.5f ? d : left_pad_.size().y * 0.5f);
    }

    if (keys_[GLFW_KEY_S]) {
        const GLfloat d = left_pad_.position().y + velocity;
        left_pad_.set_position_y(d + left_pad_.size().y * 0.5f < window_size_.y ? d : window_size_.y - left_pad_.size().y * 0.5f);
    }

    renderer.render(left_pad_);

    if (keys_[GLFW_KEY_UP]) {
        const GLfloat d = right_pad_.position().y - velocity;
        right_pad_.set_position_y(d > right_pad_.size().y * 0.5f ? d : right_pad_.size().y * 0.5f);
    }

    if (keys_[GLFW_KEY_DOWN]) {
        const GLfloat d = right_pad_.position().y + velocity;
        right_pad_.set_position_y(d + right_pad_.size().y * 0.5f < window_size_.y ? d : window_size_.y -
                right_pad_.size().y * 0.5f);
    }

    renderer.render(right_pad_);

    renderer.render(ball_);

    // check joysticks
    //if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
    //    int count;
    //    const auto axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
    //    //auto axes = std::make_unique<float[]>(glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count));

    //    if (axes[1] >= 0.5) {
    //        const GLfloat d = left_pad_.position().y + velocity;
    //        left_pad_.:set_y(d + left_pad_.size().y < win_h ? d : win_h -
    //                left_pad_.size().y);
    //    } else if (axes[1] <= -0.5) {
    //        const GLfloat d = left_pad_.position().y - velocity;
    //        left_pad_.set_y(d > 0 ? d : 0);
    //    }
    //}
}

void Simple_PingPong::game_loop(const int option) {

    start_up();

    static constexpr GLfloat bg_color[] = {0, 0, 0, 0};

    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();
        glClearBufferfv(GL_COLOR, 0, bg_color);

        render(option);

        glfwSwapBuffers(window_);
    }

    shutdown();
    //glfwDestroyWindow(window_);
    //glfwTerminate();
}

void Simple_PingPong::shutdown() {
    //glfwDestroyWindow(window_);
    //glfwTerminate();
}

void Simple_PingPong::on_key(const int key, const int action, const int scan,
        const int mods) {
    OpenGL_base::on_key(key, action, scan, mods);

    if (key >= 0 && key < static_cast<int>(keys_.size())) {
        if (action == GLFW_PRESS)
            keys_[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
            keys_[key] = GL_FALSE;
    }
}

} // end namespace OpenGL_tutorial

int main(const int argc, const char *argv[]) {

    using namespace OpenGL_tutorial;

    constexpr GLuint width {800}, height {600};

    auto game = std::make_unique<Simple_PingPong>(width, height, "Ping Pong");

    return game->run(std::move(game), argc, argv);
}

