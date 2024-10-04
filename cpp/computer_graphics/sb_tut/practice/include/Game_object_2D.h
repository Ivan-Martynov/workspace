#ifndef GAME_OBJECT_2D_H
#define GAME_OBJECT_2D_H

#include <glm/glm.hpp>

namespace OpenGL_tutorial {

class Game_object_2D {
public:
    Game_object_2D() = default;

    explicit Game_object_2D(const Game_object_2D&) = default;
    Game_object_2D& operator=(const Game_object_2D&) = default;
    explicit Game_object_2D(Game_object_2D&&) = default;
    Game_object_2D& operator=(Game_object_2D&&) = default;

    virtual ~Game_object_2D() = default;

    constexpr glm::vec2 position() const { return position_; }
    constexpr glm::vec2 size() const { return size_; }
    constexpr glm::vec4 color() const { return color_; }

    constexpr void set_position_x(const float);
    constexpr void set_position_y(const float);
    constexpr void set_position(const float, const float);
    constexpr void set_size(const float, const float);
    constexpr void set_size(const float);
    constexpr void set_color(const float, const float, const float, const float);

    virtual void scale(const float, const float);

    constexpr void move();
protected:
    constexpr void scale_position(const float, const float);
    constexpr void scale_size(const float, const float);
private:
    glm::vec2 position_ {};
    glm::vec2 size_ {};
    glm::vec4 color_ {};
};

constexpr void Game_object_2D::set_position_x(const float x) {
    position_.x = x;
}

constexpr void Game_object_2D::set_position_y(const float y) {
    position_.y = y;
}

constexpr void Game_object_2D::set_position(const float x, const float y) {
    set_position_x(x);
    set_position_y(y);
}

constexpr void Game_object_2D::set_size(const float x, const float y) {
    size_.x = x;
    size_.y = y;
}

constexpr void Game_object_2D::set_size(const float r) {
    set_size(r, r);
}

constexpr void Game_object_2D::set_color(const float r, const float g,
        const float b, const float a) {
    color_.r = r;
    color_.g = g;
    color_.b = b;
    color_.a = a;
}

constexpr void Game_object_2D::scale_position(const float ratio_x,
        const float ratio_y) {
    position_.x *= ratio_x;
    position_.y *= ratio_y;
}

constexpr void Game_object_2D::scale_size(const float ratio_x,
        const float ratio_y) {
    size_.x *= ratio_x;
    size_.y *= ratio_y;
}

void Game_object_2D::scale(const float ratio_x, const float ratio_y) {
    scale_position(ratio_x, ratio_y);
    scale_size(ratio_x, ratio_y);
}

constexpr void Game_object_2D::move() {
}


}

#endif /* GAME_OBJECT_2D_H */

