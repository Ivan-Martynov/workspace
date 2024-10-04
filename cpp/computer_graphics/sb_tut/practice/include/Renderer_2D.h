#ifndef RENDERER_2D_H
#define RENDERER_2D_H

#include <Game_object_2D.h>
#include <Vertices_helper.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

namespace OpenGL_tutorial {

class Renderer_2D {
public:
    constexpr Renderer_2D() = default;
    constexpr Renderer_2D(const GLuint program) : program_{program} { }

    explicit Renderer_2D(const Renderer_2D&) = default;
    Renderer_2D& operator=(const Renderer_2D&) = default;
    explicit Renderer_2D(Renderer_2D&&) = default;
    Renderer_2D& operator=(Renderer_2D&&) = default;

    ~Renderer_2D();

    void init(const GLuint program);

    void render(const Game_object_2D&);

private:
    GLuint vao_ {};
    GLuint buffer_ {};
    GLuint index_buffer_ {};
    GLuint program_ {};
};

void Renderer_2D::render(const Game_object_2D& game_object) {
    glUseProgram(program_);

    const auto model_location = glGetUniformLocation(program_, "model_matrix");

    auto model_mat = glm::scale(glm::translate(glm::mat4(1.0f),
                glm::vec3{game_object.position().x - game_object.size().x * 0.5,
                game_object.position().y - game_object.size().y * 0.5,
                0}),
            glm::vec3{game_object.size(), 1});

    glVertexAttrib4fv(1, glm::value_ptr(game_object.color()));
    glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model_mat));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer_2D::init(const GLuint program) {
    program_ = program;

    glCreateVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glCreateBuffers(1, &buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_);

    glCreateBuffers(1, &index_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);

    //auto vertices = quad_vertices(win_w, win_h);
    auto vertices = Vertices_helper::quad_vertices();
    auto indices = Vertices_helper::quad_indices();

    glNamedBufferStorage(buffer_, Vertices_helper::size_of_elements(vertices),
            vertices.data(), GL_MAP_WRITE_BIT);
    glNamedBufferStorage(index_buffer_,
            Vertices_helper::size_of_elements(indices), indices.data(),
            GL_MAP_WRITE_BIT);

    auto ptr = reinterpret_cast<GLfloat*>(glMapNamedBufferRange(buffer_, 0,
                Vertices_helper::size_of_elements(vertices), GL_MAP_WRITE_BIT));
    std::copy(vertices.begin(), vertices.end(), ptr);

    glUnmapNamedBuffer(buffer_);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            Vertices_helper::size_of_elements(vertices), indices.data(),
            GL_STATIC_DRAW);

    glVertexArrayVertexBuffer(vao_, 0, buffer_, 0, 2 * sizeof(GLfloat));
    glVertexArrayAttribFormat(vao_, 0, 2, GL_FLOAT, GL_FALSE, 0);

    glEnableVertexAttribArray(0);

    constexpr std::array<GLfloat, 4> color {1, 1, 1, 1};
    glVertexAttrib4fv(1, color.data());
}

Renderer_2D::~Renderer_2D() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &buffer_);
    glDeleteBuffers(1, &index_buffer_);
    glDeleteProgram(program_);
}

}

#endif /* RENDERER_2D_H */

