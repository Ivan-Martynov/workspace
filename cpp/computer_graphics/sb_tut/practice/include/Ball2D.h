#ifndef BALL2D_H
#define BALL2D_H

#include <Game_object_2D.h>

namespace OpenGL_tutorial {

class Ball_2D : public Game_object_2D {
public:
    constexpr Ball_2D() = default;

    explicit Ball_2D(const Ball_2D&) = default;
    Ball_2D& operator=(const Ball_2D&) = default;
    explicit Ball_2D(Ball_2D&&) = default;
    Ball_2D& operator=(Ball_2D&&) = default;

    ~Ball_2D() = default;

    constexpr glm::vec2 velocity() const { return velocity_; }

    constexpr void set_velocity_x(const GLfloat);
    constexpr void set_velocity_y(const GLfloat);

    constexpr void set_velocity(const GLfloat, const GLfloat);
    constexpr void set_velocity(const float);

    constexpr auto radius() { return size().x * 0.5f; }

    virtual void scale(const float, const float) override;
private:
    glm::vec2 velocity_ {};
};

constexpr void Ball_2D::set_velocity_x(const GLfloat vx) {
    velocity_.x = vx;
}

constexpr void Ball_2D::set_velocity_y(const GLfloat vy) {
    velocity_.y = vy;
}

constexpr void Ball_2D::set_velocity(const GLfloat vx, const GLfloat vy) {
    set_velocity_x(vx);
    set_velocity_y(vy);
}

constexpr void Ball_2D::set_velocity(const GLfloat v) {
    set_velocity(v, v);
}



void Ball_2D::scale(const float ratio_x, const float ratio_y) {
//    Game_object_2D::scale_position(ratio_x, ratio_y);
    Game_object_2D::scale(ratio_x, ratio_y);

    //auto average = 

//    if (size().x > size().y)
//        set_size(size().x, size().x);
//    else
//        set_size(size().y, size().y);
//
 //   if (ratio_x > ratio_y) {
 //       if (ratio_y >= 1)
 //           Game_object_2D::scale_size(ratio_y, ratio_y);
 //       else
 //           Game_object_2D::scale_size(ratio_x, ratio_x);
 //   } else {:wq

 //       if (ratio_x >= 1)
 //           Game_object_2D::scale_size(ratio_x, ratio_x);
 //       else
 //           Game_object_2D::scale_size(ratio_y, ratio_y);
 //   }
}

}

#endif /* BALL2D_H */

