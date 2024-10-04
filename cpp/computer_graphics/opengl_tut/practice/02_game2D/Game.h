#ifndef GAME_H
#define GAME_H

#include <memory>
#include <algorithm>
#include "Resource_manager.h"
#include "Sprite_renderer.h"
#include "Game_level.h"
#include "Ball_object.h"
#include "Particle_generator.h"
#include "Post_processor.h"
#include "Power_up.h"

#include <GLFW/glfw3.h>

// sound
//#include "irrklang/irrKlang.h"
#include "include/irrKlang.h"
//#include <irrKlang.h>

enum class Game_state { active, menu, win };

enum class Direction { up, right, down, left };

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

const glm::vec2 player_size {100, 20};
const GLfloat ball_radius {12.5};
const glm::vec2 ball_init_vel {100, -350};

class Game {
public:
    Game() = default;
    explicit Game(const GLuint, const GLuint);

    explicit Game(const Game&) = default;
    Game& operator=(const Game&) = default;
    explicit Game(Game&&) = default;
    Game& operator=(Game&&) = default;

    ~Game();

    void init();
    void process_input(const GLfloat);
    void update(const GLfloat);
    void render();
    void collisions();
    void reset_level();
    void reset_player();
    void spawn_power_ups(const Game_object&);
    void activate_power_up(Power_up&);
    void update_power_ups(const GLfloat);

public:
    Game_state state;
    GLboolean keys[1024];
    GLuint width, height;
    Sprite_renderer* sprend;
    std::vector<Game_level> levels;
    GLuint active_level;
    Game_object* player;
    GLfloat player_vel;
    Ball_object* ball;
    Particle_generator* particles;
    std::unique_ptr<Post_processor> effects;
    GLfloat shake_time;
    std::vector<Power_up> power_ups;
    irrklang::ISoundEngine *sound_engine;

    GLboolean check_collision(Game_object&, Game_object&);
    Collision check_collision(Ball_object&, Game_object&);
    Direction vector_direciton(const glm::vec2);
private:
    GLboolean should_spawn(const GLuint);
    GLboolean is_other_powerup_active(const std::vector<Power_up>&,
            const std::string&);
};

Game::Game(const GLuint w, const GLuint h): state {Game_state::active},
    keys {}, width {w}, height {h}, sprend {nullptr}, levels {},
    active_level {}, player {nullptr}, player_vel {500.0}, ball {nullptr},
    particles {nullptr}, effects {nullptr}, shake_time {0.0}, power_ups {},
    sound_engine {} {
    }

Game::~Game() {
    delete sprend;
    delete player;
    delete ball;
    delete particles;
    sound_engine->drop();
}

void Game::init() {
    sound_engine = irrklang::createIrrKlangDevice();
    sound_engine->play2D("sound/breakout.mp3", GL_TRUE);

    Resource_manager::load_shader("shaders/sprite.vert", "shaders/sprite.frag",
            "sprite");
    Resource_manager::load_shader("shaders/particle.vert",
            "shaders/particle.frag", "particle");
    Resource_manager::load_shader("shaders/post_process.vert",
            "shaders/post_process.frag", "postprocessing");
    glm::mat4 proj = glm::ortho(0.0f, static_cast<GLfloat>(width),
            static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);

    Shader spr_shad {Resource_manager::shader("sprite")};
    spr_shad.use();
    spr_shad.set_integer("img", 0);
    spr_shad.set_matrix4("proj", proj);

    Shader part_shad {Resource_manager::shader("particle")};
    part_shad.use();
    part_shad.set_integer("sprite_img", 0);
    part_shad.set_matrix4("proj", proj);

    sprend = new Sprite_renderer{spr_shad};
    // load textures
    Resource_manager::load_texture("textures/background.jpg", GL_FALSE,
            "background");
    Resource_manager::load_texture("textures/awesomeface.png", GL_TRUE, "face");
    Resource_manager::load_texture("textures/block.png", GL_FALSE, "block");
    Resource_manager::load_texture("textures/block_solid.png", GL_FALSE,
            "block_solid");
    Resource_manager::load_texture("textures/paddle.png", GL_TRUE, "paddle");
    Resource_manager::load_texture("textures/particle.png", GL_TRUE,
            "particle");
    Resource_manager::load_texture("textures/powerup_speed.png", GL_TRUE,
            "powerup_speed");
    Resource_manager::load_texture("textures/powerup_sticky.png", GL_TRUE,
            "powerup_sticky");
    Resource_manager::load_texture("textures/powerup_increase.png", GL_TRUE,
            "powerup_increase");
    Resource_manager::load_texture("textures/powerup_confuse.png", GL_TRUE,
            "powerup_confuse");
    Resource_manager::load_texture("textures/powerup_chaos.png", GL_TRUE,
            "powerup_chaos");
    Resource_manager::load_texture("textures/powerup_passthrough.png", GL_TRUE,
            "powerup_passthrough");

    particles = new Particle_generator(Resource_manager::shader("particle"),
            Resource_manager::texture("particle"), 500);
    effects = std::unique_ptr<Post_processor> {new Post_processor(
            Resource_manager::shader("postprocessing"), width, height)};

    Game_level lvl_one;
    lvl_one.load("levels/level_one.lvl", width, height * 0.5);
    Game_level lvl_two;
    lvl_two.load("levels/level_two.lvl", width, height * 0.5);
    Game_level lvl_three;
    lvl_three.load("levels/level_three.lvl", width, height * 0.5);
    Game_level lvl_four;
    lvl_four.load("levels/level_four.lvl", width, height * 0.5);
    levels.push_back(lvl_one);
    levels.push_back(lvl_two);
    levels.push_back(lvl_three);
    levels.push_back(lvl_four);
    active_level = 0;

    // player (paddle)
    glm::vec2 player_pos {width / 2 - (player_size.x) / 2,
        height - player_size.y};
    player = new Game_object(player_pos, player_size,
            Resource_manager::texture("paddle"));

    // ball
    const glm::vec2 ball_pos = player_pos + glm::vec2{player_size.x / 2 -
        ball_radius, -ball_radius * 2};
    ball = new Ball_object{ball_pos, ball_radius, ball_init_vel,
        Resource_manager::texture("face")};
}

void Game::process_input(const GLfloat dt) {
    if (state == Game_state::active) {
        GLfloat vel = player_vel * dt;
        if (keys[GLFW_KEY_A]) {
            if (player->pos_.x >= vel) {
                player->pos_.x -= vel;
                if (ball->is_stuck)
                    ball->pos_.x -= vel;
            }
        }
        if (keys[GLFW_KEY_D]) {
            if (player->pos_.x + vel <= width - player->size_.x) {
                player->pos_.x += vel;
                if (ball->is_stuck)
                    ball->pos_.x += vel;
            }
        }
        if (keys[GLFW_KEY_SPACE])
            ball->is_stuck = GL_FALSE;
    }
}

void Game::update(const GLfloat dt) {
    // update ball's position
    ball->move(dt, width);
    // check for collisions
    collisions();
    // particles
    particles->update(dt, *ball, 2, glm::vec2{ball->radius / 2});

    update_power_ups(dt);

    if (shake_time > 0) {
        shake_time -= dt;
        if (shake_time <= 0)
            effects->is_shake_ = GL_FALSE;
    }

    // check if the ball crosses the bottom edge
    if (ball->pos_.y >= height) {
        reset_level();
        reset_player();
    }
}

void Game::render() {
    if (state == Game_state::active) {
        effects->begin_render();

        // background
        sprend->draw_sprite(Resource_manager::texture("background"),
                glm::vec2{0, 0}, glm::vec2{width, height}, 0);

        // levels
        levels[active_level].draw(*sprend);

        // player (paddle)
        player->draw(*sprend);

        // powerups
        for (auto &p: power_ups)
            if (!p.is_destroyed_)
                p.draw(*sprend);

        // particles
        particles->draw();

        // ball
        ball->draw(*sprend);

        effects->end_render();
        effects->render(glfwGetTime());
    }
}

GLboolean Game::check_collision(Game_object& obj1, Game_object& obj2) {
    const bool coll_x = (obj1.pos_.x + obj1.size_.x >= obj2.pos_.x) &&
        (obj2.pos_.x + obj2.size_.x >= obj1.pos_.x);
    const bool coll_y = (obj1.pos_.y + obj1.size_.y >= obj2.pos_.y) &&
        (obj2.pos_.y + obj2.size_.y >= obj1.pos_.y);

    return coll_x && coll_y;
}

Collision Game::check_collision(Ball_object& obj1, Game_object& obj2) {
    using namespace glm;
    const vec2 b_center {obj1.pos_ + obj1.radius};
    const vec2 aabb_half_extents {obj2.size_.x / 2, obj2.size_.y / 2};
    const vec2 aabb_center {obj2.pos_.x + aabb_half_extents.x,
        obj2.pos_.y + aabb_half_extents.y};
    vec2 diff_vec = b_center - aabb_center;
    const vec2 clamped = clamp(diff_vec, -aabb_half_extents, aabb_half_extents);

    // get the box point closest to the ball
    const vec2 closest = aabb_center + clamped;
    diff_vec = closest - b_center;

    if (length(diff_vec) < obj1.radius)
        return std::make_tuple(GL_TRUE, vector_direciton(diff_vec), diff_vec);
    else
        return std::make_tuple(GL_FALSE, Direction::up, glm::vec2{0, 0});
}

void Game::collisions() {
    for (Game_object& box: levels[active_level].bricks_) {
        if (!box.is_destroyed_) {
            Collision coll = check_collision(*ball, box);
            if (std::get<0>(coll)) {
                if (!box.is_solid_) {
                    box.is_destroyed_ = GL_TRUE;
                    spawn_power_ups(box);
                    sound_engine->play2D("sound/bleep.mp3", GL_FALSE);
                } else {
                    shake_time = 0.05;
                    effects->is_shake_ = GL_TRUE;
                    sound_engine->play2D("sound/solid.wav", GL_FALSE);
                }
                const Direction dir = std::get<1>(coll);
                const glm::vec2 diff_vec = std::get<2>(coll);
                // horizontal collision
                if (!(ball->passthrough && !box.is_solid_)) {
                    if (dir == Direction::left || dir == Direction::right) {
                        ball->velocity_.x = -ball->velocity_.x;
                        const GLfloat pen = ball->radius - std::abs(diff_vec.x);
                        if (dir == Direction::left)
                            ball->pos_.x += pen;
                        else
                            ball->pos_.x -= pen;
                    } else { // vertical collision
                        ball->velocity_.y = -ball->velocity_.y;
                        const GLfloat pen = ball->radius - std::abs(diff_vec.y);
                        if (dir == Direction::up)
                            ball->pos_.y -= pen;
                        else
                            ball->pos_.y += pen;
                    }
                }
            }
        }
    }

    for (auto &p: power_ups) {
        if (!p.is_destroyed_) {
            if (p.pos_.y >= height)
                p.is_destroyed_ = GL_TRUE;
            if (check_collision(*player, p)) {
                activate_power_up(p);
                p.is_destroyed_ = GL_TRUE;
                p.is_activated_ = GL_TRUE;
                sound_engine->play2D("sound/powerup.wav", GL_FALSE);
            }
        }
    }

    Collision res = check_collision(*ball, *player);
    if (std::get<0>(res) && !ball->is_stuck) {
        const GLfloat pad_center {player->pos_.x + player->size_.x / 2};
        const GLfloat dist {ball->pos_.x + ball->radius - pad_center};
        const GLfloat percent {dist / (player->size_.x/2)};
        const GLfloat strength {2.0};
        const glm::vec2 old_vel = ball->velocity_;
        ball->velocity_.x = ball_init_vel.x * percent * strength;
        ball->velocity_ = glm::normalize(ball->velocity_) *
            glm::length(old_vel);
        ball->velocity_.y = -1 * std::abs(ball->velocity_.y);

        ball->is_stuck = ball->sticky;

        sound_engine->play2D("sound/bleep.wav", GL_FALSE);
    }
}

Direction Game::vector_direciton(const glm::vec2 tar) {
    const glm::vec2 compass[] = {
        glm::vec2{ 0,  1},  // north
        glm::vec2{ 1,  0},  // east
        glm::vec2{ 0, -1}, // south
        glm::vec2{-1,  0}  // west
    };
    GLfloat max_val {0};
    GLint best_match = -1;
    for (GLuint i {0}; i < 4; ++i) {
        const GLfloat dot_prod = glm::dot(glm::normalize(tar), compass[i]);
        if (dot_prod > max_val) {
            max_val = dot_prod;
            best_match = i;
        }
    }
    return static_cast<Direction>(best_match);
}

void Game::reset_level() {
    switch (active_level) {
        case 0:
            levels[0].load("levels/level_one.lvl", width, height * 0.5);
            break;
        case 1:
            levels[1].load("levels/level_two.lvl", width, height * 0.5);
            break;
        case 2:
            levels[2].load("levels/level_three.lvl", width, height * 0.5);
            break;
        case 3:
            levels[3].load("levels/level_four.lvl", width, height * 0.5);
            break;
        default:
            break;
    }
}

void Game::reset_player() {
    player->size_ = player_size;
    player->pos_ = glm::vec2 {width / 2 - (player_size.x) / 2,
        height - player_size.y};
    ball->reset(player->pos_ + glm::vec2 {player_size.x / 2 - ball_radius,
            -(ball_radius * 2)}, ball_init_vel);
}

GLboolean Game::should_spawn(const GLuint chance) {
    return (rand() % chance) == 0;
}

void Game::spawn_power_ups(const Game_object &block) {
    if (should_spawn(75))
        power_ups.push_back(Power_up {"speed", glm::vec3 {0.5, 0.5, 1}, 0,
                block.pos_, Resource_manager::texture("powerup_speed")});
    if (should_spawn(75))
        power_ups.push_back(Power_up {"sticky", glm::vec3 {1, 0.5, 1}, 20,
                block.pos_, Resource_manager::texture("powerup_sticky")});
    if (should_spawn(75))
        power_ups.push_back(Power_up {"passthrough", glm::vec3 {0.5, 1, 0.5},
                10, block.pos_,
                Resource_manager::texture("powerup_passthrough")});
    if (should_spawn(75))
        power_ups.push_back(Power_up {"pad_size_increase",
                glm::vec3 {1, 0.6, 0.4}, 0, block.pos_,
                Resource_manager::texture("powerup_increase")});
    if (should_spawn(15))
        power_ups.push_back(Power_up {"confuse", glm::vec3 {1, 0.3, 0.3}, 15,
                block.pos_, Resource_manager::texture("powerup_confuse")});
    if (should_spawn(15))
        power_ups.push_back(Power_up {"chaos", glm::vec3 {0.9, 0.25, 0.25}, 15,
                block.pos_, Resource_manager::texture("powerup_chaos")});
}

void Game::activate_power_up(Power_up& powerup) {
    const std::string p_type {powerup.type_};

    if (p_type == "speed") {
        ball->velocity_ *= 1.2;
    } else if (p_type == "sticky") {
        ball->sticky = GL_TRUE;
        player->color_ = glm::vec3{1, 0.5, 1};
    } else if (p_type == "passthrough") {
        ball->passthrough = GL_TRUE;
        ball->color_ = glm::vec3{1, 0.5, 0.5};
    } else if (p_type == "pad_size_increase") {
        player->size_.x += 50;
    } else if (p_type == "confuse") {
        if(!effects->is_chaos_)
            effects->is_confuse_ = GL_TRUE;
    } else if (p_type == "chaos") {
        if(!effects->is_confuse_)
            effects->is_chaos_ = GL_TRUE;
    }
}

GLboolean Game::is_other_powerup_active(const std::vector<Power_up>& powerups,
        const std::string& p_type) {
    for (const auto &p: powerups)
        if (p.is_activated_ && p.type_ == p_type)
            return GL_TRUE;
    return GL_FALSE;
}

void Game::update_power_ups(const GLfloat dt) {
    for (auto &p: power_ups) {
        p.pos_ += p.velocity_ * dt;
        if (p.is_activated_) {
            p.duration_ -= dt;
            if (p.duration_ <= 0) {
                // remove powerup from the list (later in the code)
                p.is_activated_ = GL_FALSE;
                // deactivate effects
                if (p.type_ == "sticky") {
                    if (!is_other_powerup_active(power_ups, "sticky")) {
                        ball->sticky = GL_FALSE;
                        player->color_ = glm::vec3{1};
                    }
                } else if (p.type_ == "passthrough") {
                    if (!is_other_powerup_active(power_ups, "passthrough")) {
                        ball->passthrough = GL_FALSE;
                        ball->color_ = glm::vec3{1};
                    }
                } else if (p.type_ == "confuse") {
                    if (!is_other_powerup_active(power_ups, "confuse")) {
                        // only reset if no other powerup of type confuse
                        effects->is_confuse_ = GL_FALSE;
                    }
                } else if (p.type_ == "chaos") {
                    if (!is_other_powerup_active(power_ups, "chaos")) {
                        // only reset if no other powerup of type chaos
                        effects->is_chaos_ = GL_FALSE;
                    }
                }
            }
        }
    }
    // remove all detroyed and !activated powerups
    power_ups.erase(std::remove_if(std::begin(power_ups), std::end(power_ups),
                [](const Power_up& p) { return p.is_destroyed_ &&
                !p.is_activated_; }), std::end(power_ups));
}

#endif /* GAME_H */

