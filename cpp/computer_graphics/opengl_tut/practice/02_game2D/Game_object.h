#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Texture.h"
#include "Sprite_renderer.h"

// GL math
//#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Game_object {
public:
    Game_object();
    Game_object(const glm::vec2, const glm::vec2, const Texture2D&,
            glm::vec3 = glm::vec3{1}, glm::vec2 = glm::vec2{0, 0});

    explicit Game_object(const Game_object&) = default;
    Game_object& operator=(const Game_object&) = default;
    explicit Game_object(Game_object&&) = default;
    Game_object& operator=(Game_object&&) = default;

    virtual ~Game_object() = default;

    virtual void draw(Sprite_renderer&);

//private:
public:
    glm::vec2 pos_, size_, velocity_;
    glm::vec3 color_;
    GLfloat rotation_;
    GLboolean is_solid_;
    GLboolean is_destroyed_;
    // render state
    Texture2D sprite_;
};

Game_object::Game_object(): pos_ {0, 0}, size_ {1, 1}, velocity_ {0},
    color_ {1}, rotation_ {0}, is_solid_ {false}, is_destroyed_ {false},
    sprite_ {} {
    }

Game_object::Game_object(const glm::vec2 pos, const glm::vec2 size,
        const Texture2D& sprite, glm::vec3 color, glm::vec2 vel): pos_ {pos},
    size_ {size}, velocity_ {vel}, color_ {color}, rotation_ {0},
    is_solid_ {false}, is_destroyed_ {false}, sprite_ {sprite} {
    }

void Game_object::draw(Sprite_renderer& rend) {
    rend.draw_sprite(sprite_, pos_, size_, rotation_, color_);
}

#endif /* GAME_OBJECT_H */

