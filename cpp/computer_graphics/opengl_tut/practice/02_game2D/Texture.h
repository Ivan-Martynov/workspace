#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

class Texture2D {
public:
    explicit Texture2D();

    //explicit Texture2D(const Texture2D&) = default;
    //Texture2D& operator=(const Texture2D&) = default;
    //explicit Texture2D(Texture2D&&) = default;
    //Texture2D& operator=(Texture2D&&) = default;

    ~Texture2D() = default;

    void generate(const GLuint, const GLuint, unsigned char*);
    void bind() const;

public:
    GLuint id;
    GLuint width, height;
    GLuint internal_format;
    GLuint image_format;
    GLuint wrap_s, wrap_t;
    GLuint filter_min, filter_max;
};

Texture2D::Texture2D(): id {}, width {0}, height {0}, internal_format{GL_RGB},
    image_format {GL_RGB}, wrap_s {GL_REPEAT}, wrap_t {GL_REPEAT},
    filter_min {GL_LINEAR}, filter_max {GL_LINEAR} {
        glGenTextures(1, &id);
    }

void Texture2D::generate(const GLuint w, const GLuint h, unsigned char* data) {
    width  = w;
    height = h;
    // create texture
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0,
            image_format, GL_UNSIGNED_BYTE, data);
    // set texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind() const {
    glBindTexture(GL_TEXTURE_2D, id);
}

#endif /* TEXTURE_H */

