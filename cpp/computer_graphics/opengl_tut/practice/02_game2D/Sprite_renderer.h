#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Shader.h"
#include "Texture.h"

// GL math
//#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sprite_renderer {
public:
    Sprite_renderer(const Shader&);

   ~Sprite_renderer();

   void draw_sprite(const Texture2D&, const glm::vec2,
           const glm::vec2 = glm::vec2{10, 10}, const GLfloat = 1,
           const glm::vec3 = glm::vec3{1});

private:
   Shader shader_;
   GLuint quad_vao_;

   void init_render_data();
};

Sprite_renderer::Sprite_renderer(const Shader& shad): shader_ {shad},
    quad_vao_ {} {
    init_render_data();
}

Sprite_renderer::~Sprite_renderer() {
    glDeleteVertexArrays(1, &quad_vao_);
}

void Sprite_renderer::draw_sprite(const Texture2D& tex, const glm::vec2 pos,
        const glm::vec2 size, const GLfloat rot, const glm::vec3 color_vec) {
    shader_.use();

    using namespace glm;
    mat4 model_mat = translate(mat4{}, vec3{pos, 0});
    model_mat = translate(model_mat, vec3{0.5 * size.x, 0.5 * size.y, 0});
    model_mat = rotate(model_mat, rot, vec3{0, 0, 1});
    model_mat = translate(model_mat, vec3{-0.5 * size.x, -0.5 * size.y, 0});
    model_mat = scale(model_mat, vec3{size, 1});

    shader_.set_matrix4("model", model_mat);
    shader_.set_vector3f("sprite_color", color_vec);

    glActiveTexture(GL_TEXTURE0);
    tex.bind();

    glBindVertexArray(quad_vao_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Sprite_renderer::init_render_data() {
    GLuint vbo;
    const GLfloat vertices[] = {
        // pos  tex
        0, 1,   0, 1,
        1, 0,   1, 0,
        0, 0,   0, 0,

        0, 1,   0, 1,
        1, 1,   1, 1,
        1, 0,   1, 0
    };

    glGenVertexArrays(1, &quad_vao_);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(quad_vao_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) << 2,
            (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

#endif /* SPRITE_RENDERER_H */

