#ifndef BALL_OBJECT_H
#define BALL_OBJECT_H

#include "Game_object.h"

class Ball_object: public Game_object {
public:
    Ball_object();
    Ball_object(const glm::vec2, const GLfloat, const glm::vec2,
            const Texture2D&);

    explicit Ball_object(const Ball_object&) = default;
    Ball_object& operator=(const Ball_object&) = default;
    explicit Ball_object(Ball_object&&) = default;
    Ball_object& operator=(Ball_object&&) = default;

    ~Ball_object() = default;

    glm::vec2 move(const GLfloat, const GLuint);
    void reset(const glm::vec2, glm::vec2);

public:
    GLfloat radius;
    GLboolean is_stuck;
    GLboolean sticky, passthrough;
};

Ball_object::Ball_object(): Game_object(), radius {12.5}, is_stuck {GL_TRUE},
    sticky {GL_FALSE}, passthrough {GL_FALSE} {
}

Ball_object::Ball_object(const glm::vec2 pos, const GLfloat rad,
        const glm::vec2 vel, const Texture2D& sprite):
    Game_object(pos, glm::vec2{rad * 2, rad * 2}, sprite, glm::vec3{1}, vel),
    radius {rad}, is_stuck {GL_TRUE}, sticky {GL_FALSE}, passthrough {GL_FALSE}
{
}

glm::vec2 Ball_object::move(const GLfloat dt, const GLuint win_width) {
    if (!is_stuck) {
        pos_ += velocity_ * dt;
        if (pos_.x <= 0) {
            velocity_.x = -velocity_.x;
            pos_.x = 0;
        } else if (pos_.x + size_.x >= win_width) {
            velocity_.x = -velocity_.x;
            pos_.x = win_width - size_.x;
        }
        if (pos_.y <= 0) {
            velocity_.y = -velocity_.y;
            pos_.y = 0;
        }
    }
    return pos_;
}

void Ball_object::reset(const glm::vec2 pos, const glm::vec2 vel) {
    pos_ = pos;
    velocity_ = vel;
    is_stuck = GL_TRUE;
}

#endif /* BALL_OBJECT_H */

