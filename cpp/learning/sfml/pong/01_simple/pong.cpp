#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <functional>
#include <chrono>

constexpr std::uint16_t window_width {1600};
constexpr std::uint16_t window_height {900};

constexpr float update_ms {33};

constexpr float ball_radius {10};
constexpr float ball_velocity {400};
constexpr float ball_velocity_increment {60};

constexpr float paddle_width {10};
constexpr float paddle_length {50};
constexpr float paddle_padding {20};
constexpr float paddle_speed {400};

constexpr float court_margin {10};
constexpr float court_outline_width {5};


enum class GameState : std::uint_fast8_t
{
    MENU,
    IN_GAME,
    EXIT,
};

enum class PlayState : std::uint_fast8_t
{
    SERVE_PLAYER_ONE,
    SERVE_PLAYER_TWO,
    TOWARD_PLAYER_ONE,
    TOWARD_PLAYER_TWO,
};

enum class MouseState : std::uint_fast8_t
{
    UP,
    DOWN,
};

struct Vector2d
{
    float x;
    float y;
};

struct RectangleShape
{
    float x;
    float y;
    float width;
    float height;
};

class Court
{
  public:
    Court(const RectangleShape& dimensions) : m_dimensions {dimensions} {}

    const RectangleShape& get_dimensions() const
    {
        return m_dimensions;
    }

  private:
    RectangleShape m_dimensions;
};

class Paddle
{
  public:
    Paddle(const RectangleShape& starting_position) : m_rect {starting_position}
    {
    }

    const RectangleShape& get_position_size() const
    {
        return m_rect;
    }

    void set_position_size(const RectangleShape& position_size)
    {
        m_rect = position_size;
    }

    void set_position(const Vector2d& position)
    {
        m_rect.x = position.x;
        m_rect.y = position.y;
    }

  private:
    RectangleShape m_rect;
};

class Ball
{
  private:
    Vector2d m_position;
    float m_radius;
    Vector2d m_velocity;

  public:
    Ball(const Vector2d& position, const float radius)
        : m_position {position}, m_radius {radius}, m_velocity {0, 0}
    {
    }

    const Vector2d& get_position() const
    {
        return m_position;
    }

    const float& get_radius() const
    {
        return m_radius;
    }

    const Vector2d& get_velocity() const
    {
        return m_velocity;
    }

    void set_position(const Vector2d& position)
    {
        m_position = position;
    }

    void set_velocity(const Vector2d& velocity)
    {
        m_velocity = velocity;
    }
};

class GameRenderer
{
  private:
    static sf::RenderTarget* m_target_ptr;
    static sf::Font* m_font_ptr;

  public:
    static bool init(sf::RenderTarget* target_ptr, sf::Font* font_ptr)
    {
        m_target_ptr = target_ptr;
        m_font_ptr = font_ptr;
        return true;
    }

    static void Render(const float&, const Paddle& player_one,
        const Paddle& player_two, const Ball& ball, const Court& court,
        const std::uint_fast8_t& score1, const std::uint_fast8_t& score2)
    {
        sf::RectangleShape court_shape {};
        const RectangleShape& cShape {court.get_dimensions()};
        court_shape.setPosition({cShape.x, cShape.y});
        court_shape.setSize({cShape.width, cShape.height});
        court_shape.setFillColor(sf::Color::Transparent);
        court_shape.setOutlineColor(sf::Color::White);
        court_shape.setOutlineThickness(-court_outline_width);
        m_target_ptr->draw(court_shape);

        court_shape.setPosition(
            {(window_width - court_outline_width) / 2, court_margin});
        court_shape.setSize(
            {court_outline_width, window_height - court_margin * 2});
        m_target_ptr->draw(court_shape);

        sf::RectangleShape paddle_shape {};
        const RectangleShape& p1Shape {player_one.get_position_size()};
        paddle_shape.setPosition({p1Shape.x, p1Shape.y});
        paddle_shape.setSize({p1Shape.width, p1Shape.height});
        paddle_shape.setFillColor(sf::Color::White);
        m_target_ptr->draw(paddle_shape);

        const RectangleShape& p2Shape {player_two.get_position_size()};
        paddle_shape.setPosition({p2Shape.x, p2Shape.y});
        paddle_shape.setSize({p2Shape.width, p2Shape.height});
        paddle_shape.setFillColor(sf::Color::White);
        m_target_ptr->draw(paddle_shape);

        sf::CircleShape ball_shape {};
        const Vector2d& ball_position {ball.get_position()};
        const float& radius {ball.get_radius()};
        ball_shape.setPosition(
            {ball_position.x - ball_radius, ball_position.y - ball_radius});
        ball_shape.setRadius(radius);
        ball_shape.setFillColor(sf::Color::White);
        m_target_ptr->draw(ball_shape);

        sf::Text score_text {
            std::to_string(score1) + "\t" + std::to_string(score2), *m_font_ptr,
            40};
        sf::FloatRect bounds {score_text.getLocalBounds()};
        score_text.setPosition({(window_width - bounds.width) / 2,
            court_margin + court_outline_width + 5});
        m_target_ptr->draw(score_text);
    }
};

sf::RenderTarget* GameRenderer::m_target_ptr = nullptr;
sf::Font* GameRenderer::m_font_ptr = nullptr;

class PongGame
{
  private:
    std::uint_fast8_t m_player_one_score;
    std::uint_fast8_t m_player_two_score;
    std::uint_fast8_t m_max_score;
    Court m_court;
    Ball m_ball;
    Paddle m_player_one;
    Paddle m_player_two;
    PlayState m_play_state;

  public:
    PongGame(const std::uint_fast8_t score_to_win, sf::RenderTarget& target,
        sf::Font& font)
        : m_player_one_score {0}, m_player_two_score {0},
          m_max_score {score_to_win},
          m_court {{court_margin, court_margin, window_width - court_margin * 2,
              window_height - court_margin * 2}},
          m_ball {{window_width / 2, window_height / 2}, ball_radius},
          m_player_one {{court_margin + paddle_padding,
              (window_height - paddle_length) / 2, paddle_width,
              paddle_length}},
          m_player_two {
              {window_width - court_margin - paddle_padding - paddle_width,
                  (window_height - paddle_length) / 2, paddle_width,
                  paddle_length}},
          m_play_state {PlayState::SERVE_PLAYER_ONE}
    {
        GameRenderer::init(&target, &font);
    }

    GameState update(const float elapsed_ms)
    {
        const float time_multiplier {elapsed_ms * 0.0001f};

        const RectangleShape& paddle1 {m_player_one.get_position_size()};
        const RectangleShape& paddle2 {m_player_two.get_position_size()};

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            m_player_one.set_position(
                {paddle1.x, paddle1.y - paddle_speed * time_multiplier});
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            m_player_one.set_position(
                {paddle1.x, paddle1.y + paddle_speed * time_multiplier});
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            m_player_two.set_position(
                {paddle2.x, paddle2.y - paddle_speed * time_multiplier});
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            m_player_two.set_position(
                {paddle2.x, paddle2.y + paddle_speed * time_multiplier});
        }

        switch (m_play_state)
        {
            case PlayState::SERVE_PLAYER_ONE:
            {
                m_ball.set_velocity({0, 0});
                const RectangleShape& paddle {m_player_one.get_position_size()};
                m_ball.set_position(
                    {paddle.x + paddle_width, paddle.y + paddle_length / 2});
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {
                    m_ball.set_velocity({ball_velocity, 0});
                    m_play_state = PlayState::TOWARD_PLAYER_TWO;
                }
            }
            break;

            case PlayState::SERVE_PLAYER_TWO:
            {
                m_ball.set_velocity({0, 0});
                const RectangleShape& paddle {m_player_two.get_position_size()};
                m_ball.set_position(
                    {paddle.x + ball_radius, paddle.y + paddle_length / 2});
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {
                    m_ball.set_velocity({-ball_velocity, 0});
                    m_play_state = PlayState::TOWARD_PLAYER_ONE;
                }
            }
            break;

            case PlayState::TOWARD_PLAYER_ONE:
                break;

            case PlayState::TOWARD_PLAYER_TWO:
                break;

            default:
                break;
        }

        Vector2d ball_pos {m_ball.get_position()};
        Vector2d ball_vel {m_ball.get_velocity()};
        const RectangleShape& court_shape {m_court.get_dimensions()};

        ball_pos.x += ball_vel.x * time_multiplier;
        ball_pos.y += ball_vel.y * time_multiplier;

        m_ball.set_position(ball_pos);

        switch (m_play_state)
        {
            case PlayState::TOWARD_PLAYER_ONE:
            {
                if (ball_pos.x - ball_radius > paddle1.x + paddle_width)
                {
                    break;
                }

                if ((ball_pos.y + ball_radius >= paddle1.y)
                    && (ball_pos.y - ball_radius <= paddle1.y + paddle_length))
                {
                    m_ball.set_position(
                        {paddle1.x + paddle_width + ball_radius + 1,
                            ball_pos.y});
                    ball_vel.x = -ball_vel.x;

                    if (ball_pos.y + ball_radius
                        <= paddle1.y + paddle_length / 3)
                    {
                        ball_vel.y -= ball_velocity / 2;
                    }
                    else if (ball_pos.y - ball_radius
                             >= paddle1.y + paddle_length / 3 * 2)
                    {
                        ball_vel.y += ball_velocity / 2;
                    }

                    if (ball_vel.x > 0)
                    {
                        ball_vel.x += ball_velocity_increment;
                    }
                    else
                    {
                        ball_vel.x -= ball_velocity_increment;
                    }

                    if (ball_vel.y > 0)
                    {
                        ball_vel.y += ball_velocity_increment;
                    }
                    else
                    {
                        ball_vel.y -= ball_velocity_increment;
                    }

                    m_ball.set_velocity(ball_vel);
                    m_play_state = PlayState::TOWARD_PLAYER_TWO;

                    break;
                }

                if (ball_pos.x + ball_radius < paddle1.x)
                {
                    ++m_player_two_score;
                    m_play_state = PlayState::SERVE_PLAYER_ONE;
                }
            }
            break;

            case PlayState::TOWARD_PLAYER_TWO:
            {
                if (ball_pos.x + ball_radius < paddle2.x)
                {
                    break;
                }

                if ((ball_pos.y + ball_radius >= paddle2.y)
                    && (ball_pos.y - ball_radius <= paddle2.y + paddle_length))
                {
                    m_ball.set_position(
                        {paddle2.x - ball_radius - 1, ball_pos.y});
                    ball_vel.x = -ball_vel.x;

                    if (ball_pos.y + ball_radius
                        <= paddle2.y + paddle_length / 3)
                    {
                        ball_vel.y -= ball_velocity / 2;
                    }
                    else if (ball_pos.y - ball_radius
                             >= paddle2.y + paddle_length / 3 * 2)
                    {
                        ball_vel.y += ball_velocity / 2;
                    }

                    if (ball_vel.x > 0)
                    {
                        ball_vel.x += ball_velocity_increment;
                    }
                    else
                    {
                        ball_vel.x -= ball_velocity_increment;
                    }

                    if (ball_vel.y > 0)
                    {
                        ball_vel.y += ball_velocity_increment;
                    }
                    else
                    {
                        ball_vel.y -= ball_velocity_increment;
                    }

                    m_ball.set_velocity(ball_vel);
                    m_play_state = PlayState::TOWARD_PLAYER_ONE;

                    break;
                }

                if (ball_pos.x - ball_radius > paddle2.x + paddle_width)
                {
                    ++m_player_one_score;
                    m_play_state = PlayState::SERVE_PLAYER_TWO;
                }
            }
            break;

            default:
                break;
        }

        if (ball_pos.y <= court_shape.y)
        {
            m_ball.set_position({ball_pos.x, court_shape.y});
            m_ball.set_velocity({ball_vel.x, -ball_vel.y});
        }
        else if (ball_pos.y >= court_shape.y + court_shape.height)
        {
            m_ball.set_position(
                {ball_pos.x, court_shape.y + court_shape.height});
            m_ball.set_velocity({ball_vel.x, -ball_vel.y});
        }

        if ((m_player_one_score >= m_max_score)
            || (m_player_two_score >= m_max_score))
        {
            return GameState::MENU;
        }
        else
        {
            return GameState::IN_GAME;
        }
    }

    void Render(const float elapsed_ms) const
    {
        GameRenderer::Render(elapsed_ms, m_player_one, m_player_two, m_ball,
            m_court, m_player_one_score, m_player_two_score);
    }
};

class Button
{
  public:
    typedef std::function<void(void)> CallbackFunc;

    enum class State : std::uint_fast8_t
    {
        UP,
        DOWN,
        HOVER
    };

    Button(const std::string& text, const RectangleShape& position_and_size)
        : m_text {text}, m_position_and_size {position_and_size},
          m_color_up {sf::Color::Black}, m_color_down {sf::Color::Red},
          m_color_hover {sf::Color::Yellow}, m_callback([]() {}),
          m_state(State::UP)
    {
    }

    const RectangleShape& get_position_and_size() const
    {
        return m_position_and_size;
    }

    void set_position_and_size(const RectangleShape& position_and_size)
    {
        m_position_and_size = position_and_size;
    }

    void set_position(const Vector2d& position)
    {
        m_position_and_size.x = position.x;
        m_position_and_size.y = position.y;
    }

    void set_size(const Vector2d& size)
    {
        m_position_and_size.width = size.x;
        m_position_and_size.height = size.y;
    }

    bool handle_input(
        const Vector2d& mouse_position, const MouseState& mouse_state)
    {
        if ((mouse_position.x >= m_position_and_size.x)
            && (mouse_position.x
                <= m_position_and_size.x + m_position_and_size.width)
            && (mouse_position.y >= m_position_and_size.y)
            && (mouse_position.y
                <= m_position_and_size.y + m_position_and_size.height))
        {
            m_state = State::HOVER;
        }
        else
        {
            m_state = State::UP;
        }

        if ((mouse_state == MouseState::DOWN) && (m_state == State::HOVER))
        {
            m_state = State::DOWN;
            m_callback();
            return true;
        }

        return false;
    }

    void set_colors(
        const sf::Color& up, const sf::Color& down, const sf::Color& hover)
    {
        m_color_up = up;
        m_color_down = down;
        m_color_hover = hover;
    }

    void set_callback(const CallbackFunc callback)
    {
        m_callback = callback;
    }

    const Button::State& get_state() const
    {
        return m_state;
    }

    void set_state(const Button::State& state)
    {
        m_state = state;
    }

    void Render(sf::RenderTarget& target, const sf::Font& font) const
    {
        sf::Text button_text {m_text, font, 60};
        if (m_state == State::DOWN)
        {
            //button_text.setColor(m_color_down);
        }
        else if (m_state == State::HOVER)
        {
            //button_text.setColor(m_color_hover);
        }
        else
        {
            //button_text.setColor(m_color_up);
        }

        button_text.setPosition({m_position_and_size.x, m_position_and_size.y});

        sf::RectangleShape bg {};
        bg.setPosition({m_position_and_size.x, m_position_and_size.y});
        bg.setSize({m_position_and_size.width, m_position_and_size.height});
        bg.setFillColor(sf::Color::White);

        target.draw(bg);
        target.draw(button_text);
    }

  private:
    std::string m_text;
    RectangleShape m_position_and_size;
    sf::Color m_color_up;
    sf::Color m_color_down;
    sf::Color m_color_hover;
    CallbackFunc m_callback;
    State m_state;
};

class PongMenu
{
    private:
      sf::RenderTarget& m_target;
      sf::Font& m_font;

      Button m_play_button;
      Button m_exit_button;

      bool m_should_exit;
      bool m_should_start;

    public:
      PongMenu(sf::RenderTarget& target, sf::Font& font)
          : m_target {target}, m_font {font},
            m_play_button {
                "PLAY", {window_width / 2, window_height / 2, 140, 65}},
            m_exit_button {
                "EXIT", {window_width / 2, window_height / 2 + 100, 130, 65}},
            m_should_exit {false}, m_should_start {false}
      {
          m_play_button.set_callback([this]() { m_should_start = true; });
          m_exit_button.set_callback([this]() { m_should_exit = true; });
      }

      GameState update(const float, const Vector2d& mouse_pos)
      {
          MouseState state {MouseState::UP};
          if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
              || sf::Mouse::isButtonPressed(sf::Mouse::Middle)
              || sf::Mouse::isButtonPressed(sf::Mouse::Right))
          {
              state = MouseState::DOWN;
          }

          m_play_button.handle_input(mouse_pos, state);
          m_exit_button.handle_input(mouse_pos, state);

          if (m_should_exit)
          {
              return GameState::EXIT;
          }

          if (m_should_start)
          {
              return GameState::IN_GAME;
          }

          return GameState::MENU;
      }

      void Render(const float) const
      {
          m_play_button.Render(m_target, m_font);
          m_exit_button.Render(m_target, m_font);
      }

      void Reset()
      {
          m_should_exit = false;
          m_should_start = false;
      }
};

int main()
{
    sf::RenderWindow window {
        sf::VideoMode {window_width, window_height}, "Pong"};

    sf::Font font {};

    GameState game_state {GameState::MENU};

    PongGame pong {3, window, font};
    PongMenu menu {window, font};

    auto last_time {std::chrono::system_clock::now()};
    float frame_lag {0.0f};

    while (window.isOpen() && game_state != GameState::EXIT)
    {
        sf::Event event {};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            auto current_time {std::chrono::system_clock::now()};
            const auto elapsed_ms {
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    current_time - last_time)};
            last_time = current_time;
            frame_lag += elapsed_ms.count();

            sf::Vector2i mouse_pos {sf::Mouse::getPosition(window)};
            while (frame_lag >= update_ms)
            {
                frame_lag -= update_ms;

                if (game_state == GameState::MENU)
                {
                    game_state = menu.update(
                        update_ms, {static_cast<float>(mouse_pos.x),
                                       static_cast<float>(mouse_pos.y)});
                }
                else
                {
                    game_state = pong.update(update_ms);
                    if (game_state == GameState::MENU)
                    {
                        menu.Reset();
                    }
                }
            }

            window.clear();

            if (game_state == GameState::MENU)
            {
                menu.Render(elapsed_ms.count());
            }
            else
            {
                pong.Render(elapsed_ms.count());
            }

            window.display();
        }
    }

    window.close();

    return 0;
}
