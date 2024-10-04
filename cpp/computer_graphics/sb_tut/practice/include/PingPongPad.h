#ifndef PINGPONGPAD_H
#define PINGPONGPAD_H

#include <Game_object_2D.h>

namespace OpenGL_tutorial {

class PingPongPad : public Game_object_2D {
public:
    constexpr PingPongPad() = default;

    explicit PingPongPad(const PingPongPad&) = default;
    PingPongPad& operator=(const PingPongPad&) = default;
    explicit PingPongPad(PingPongPad&&) = default;
    PingPongPad& operator=(PingPongPad&&) = default;

    ~PingPongPad() = default;

    constexpr float velocity() const { return velocity_; }

    void move();
private:
    float velocity_ {};
};

void PingPongPad::move() {
}

}

#endif /* PINGPONGPAD_H */

