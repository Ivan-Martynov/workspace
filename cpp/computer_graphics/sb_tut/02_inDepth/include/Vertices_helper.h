#ifndef VERTICES_HELPER_H
#define VERTICES_HELPER_H

#include <array>

namespace OpenGL_tutorial {

class Vertices_helper {
public:
    static constexpr std::array<float, 108> cube_vertices();
private:
    Vertices_helper() { }
};

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

