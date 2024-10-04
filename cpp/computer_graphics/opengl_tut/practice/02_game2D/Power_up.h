#ifndef POWER_UP_H
#define POWER_UP_H

#include "Game_object.h"

class Power_up: public Game_object {
public:
    std::string type_;
    GLfloat duration_;
    GLboolean is_activated_;
    static const glm::vec2 size_;
    static const glm::vec2 velocity_;

    Power_up() = default;
    Power_up(const std::string&, const glm::vec3, const GLfloat,
            const glm::vec2, const Texture2D&);

    explicit Power_up(const Power_up&) = default;
    Power_up& operator=(const Power_up&) = default;
    explicit Power_up(Power_up&&) = default;
    Power_up& operator=(Power_up&&) = default;

    ~Power_up() = default;
};

const glm::vec2 Power_up::size_ {60, 20};
const glm::vec2 Power_up::velocity_ {0, 150};

Power_up::Power_up(const std::string& type, const glm::vec3 clr,
        const GLfloat duration, const glm::vec2 pos, const Texture2D& tex):
    Game_object {pos, size_, tex, clr, velocity_}, type_ {type},
    duration_ {duration}, is_activated_ {} {
    }

#endif /* POWER_UP_H */

