#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H

#include <vector>
#include "Shader.h"
#include "Texture.h"
#include "Game_object.h"

struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec4 color;
    GLfloat life;

    Particle(): position {0}, velocity {0}, color {1}, life {0} { }
};

class Particle_generator {
public:
    Particle_generator() = default;
    Particle_generator(const Shader&, const Texture2D&, const GLuint);

    explicit Particle_generator(const Particle_generator&) = default;
    Particle_generator& operator=(const Particle_generator&) = default;
    explicit Particle_generator(Particle_generator&&) = default;
    Particle_generator& operator=(Particle_generator&&) = default;

    ~Particle_generator() = default;

    void update(const GLfloat, Game_object&, const GLuint,
            const glm::vec2 = glm::vec2{0, 0});

    void draw();
private:
    // state
    std::vector<Particle> particles_;
    GLuint amount_;

    // render state
    Shader shader_;
    Texture2D texture_;
    GLuint vao_;
    GLuint last_;

    // initialize buffer and vertex attributes
    void init();
    GLuint first_unused_particle();
    void respawn_particle(Particle&, Game_object&,
            const glm::vec2 = glm::vec2 {0, 0});
};

Particle_generator::Particle_generator(const Shader& shad, const Texture2D& tex,
        const GLuint amount): particles_ {}, amount_ {amount}, shader_ {shad},
    texture_ {tex}, vao_ {}, last_ {0} {
        init();
    }

void Particle_generator::update(const GLfloat dt, Game_object& obj,
        const GLuint new_particles, const glm::vec2 offset_vec) {
    // add new particles
    for (GLuint i {0}; i < new_particles; ++i) {
        const GLuint unused_part {first_unused_particle()};
        respawn_particle(particles_[unused_part], obj, offset_vec);
    }
    // update all particles
    for (auto &p: particles_) {
        p.life -= dt;
        if (p.life > 0) {
            p.position -= p.velocity * dt;
            p.color.a -= dt * 2.5;
        }
    }
}

void Particle_generator::draw() {
    // use additive blending for a glow effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    shader_.use();
    for (const auto p: particles_) {
        if (p.life > 0) {
            shader_.set_vector2f("offset", p.position);
            shader_.set_vector4f("color", p.color);
            texture_.bind();
            glBindVertexArray(vao_);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    // reset the blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Particle_generator::init() {
    GLuint vbo;
    GLfloat particle_quad[] = {
        0, 1, 0, 1,
        1, 0, 1, 0,
        0, 0, 0, 0,

        0, 1, 0, 1,
        1, 1, 1, 1,
        1, 0, 1, 0
    };
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao_);
    // fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad,
            GL_STATIC_DRAW);

    // set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) << 2,
            (GLvoid*)0);

    // create amount_ default particle instances
    for (GLuint i {0}; i < amount_; ++i)
        particles_.push_back(Particle{});
}

GLuint Particle_generator::first_unused_particle() {
    // search from the last used particle
    for (GLuint i {last_}; i < amount_; ++i)
        if (particles_[i].life <= 0) {
            last_ = i;
            return i;
        }
    // otherwise, linear search
    for (GLuint i {0}; i < last_; ++i)
        if (particles_[i].life <= 0) {
            last_ = i;
            return i;
        }
    last_ = 0;
    return 0;
}

void Particle_generator::respawn_particle(Particle& part, Game_object& obj,
            const glm::vec2 offset_vec) {
    GLfloat rand_val {((rand() % 100) - 50) / 10.0f};
    GLfloat r_color {0.5f + (rand() % 100) / 100.0f};
    part.position = obj.pos_ + rand_val + offset_vec;
    part.color = glm::vec4{r_color, r_color, r_color, 1};
    part.life = 1;
    part.velocity = obj.velocity_ * 0.1f;
}

#endif /* PARTICLE_GENERATOR_H */

