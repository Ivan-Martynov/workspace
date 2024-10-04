#ifndef VERTICES_HELPER_H
#define VERTICES_HELPER_H

#include <vector>
#include <array>

#include <glad/glad.h>

namespace OpenGL_tutorial {

class Vertices_helper {
public:
    static constexpr std::array<float, 108> cube_vertices();
    static constexpr std::array<GLfloat, 8> quad_vertices();
    static constexpr std::array<GLuint, 6> quad_indices();

    template <class T>
        static constexpr size_t size_of_elements(const std::vector<T>&);
    template <class T, size_t N>
        static constexpr size_t size_of_elements(const std::array<T, N>&);
private:
    Vertices_helper() { }
};

// function to compute sizeof elements lying in the vector container
template <class T>
constexpr size_t Vertices_helper::size_of_elements(const std::vector<T>& v) {
    return v.size() * sizeof(T);
}

// function to compute sizeof elements lying in the vector container
template <class T, size_t N>
constexpr size_t Vertices_helper::size_of_elements(const std::array<T, N>&) {
    return N * sizeof(T);
}

constexpr std::array<GLfloat, 8> Vertices_helper::quad_vertices() {
    return std::array<GLfloat, 8> {
        0, 0,
        0, 1,
        1, 1,
        1, 0
    };
}

constexpr std::array<GLuint, 6> Vertices_helper::quad_indices() {
    return std::array<GLuint, 6> {
        0, 1, 3,
        1, 2, 3
    };
}

constexpr std::array<float, 108> Vertices_helper::cube_vertices() {
    return std::array<float, 108> {
        -0.25f,  0.25f, -0.25f,
        -0.25f, -0.25f, -0.25f,
         0.25f, -0.25f, -0.25f,
         0.25f, -0.25f, -0.25f,
         0.25f,  0.25f, -0.25f,
        -0.25f,  0.25f, -0.25f,

         0.25f, -0.25f, -0.25f,
         0.25f, -0.25f,  0.25f,
         0.25f,  0.25f, -0.25f,
         0.25f, -0.25f,  0.25f,
         0.25f,  0.25f,  0.25f,
         0.25f,  0.25f, -0.25f,

         0.25f, -0.25f,  0.25f,
        -0.25f, -0.25f,  0.25f,
         0.25f,  0.25f,  0.25f,
        -0.25f, -0.25f,  0.25f,
        -0.25f,  0.25f,  0.25f,
         0.25f,  0.25f,  0.25f,

        -0.25f, -0.25f,  0.25f,
        -0.25f, -0.25f, -0.25f,
        -0.25f,  0.25f,  0.25f,
        -0.25f, -0.25f, -0.25f,
        -0.25f,  0.25f, -0.25f,
        -0.25f,  0.25f,  0.25f,

        -0.25f, -0.25f,  0.25f,
         0.25f, -0.25f,  0.25f,
         0.25f, -0.25f, -0.25f,
         0.25f, -0.25f, -0.25f,
        -0.25f, -0.25f, -0.25f,
        -0.25f, -0.25f,  0.25f,

        -0.25f,  0.25f, -0.25f,
         0.25f,  0.25f, -0.25f,
         0.25f,  0.25f,  0.25f,
         0.25f,  0.25f,  0.25f,
        -0.25f,  0.25f,  0.25f,
        -0.25f,  0.25f, -0.25f
    };
}

}

#endif /* VERTICES_HELPER_H */

