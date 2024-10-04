#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <SOIL/SOIL.h>
#include "Shader.h"
#include "Texture.h"

class Resource_manager {
public:
    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Texture2D> textures;

    ~Resource_manager() = default;

    static Shader load_shader(const std::string&, const std::string&,
            const std::string&);
    static Shader load_shader(const std::string&, const std::string&,
            const std::string&, const std::string&);
    static Shader shader(const std::string&);

    static Texture2D load_texture(const std::string&, const GLboolean,
            const std::string&);
    static Texture2D texture(const std::string&);

    static void clear();
private:
    Resource_manager() { };
    static Texture2D load_texture_file(const std::string&, const GLboolean);
};

// instantiate static variables
std::map<std::string, Shader> Resource_manager::shaders;
std::map<std::string, Texture2D> Resource_manager::textures;

// load shader from vertex and fragment shader files
Shader Resource_manager::load_shader(const std::string& vert,
        const std::string& frag, const std::string& name) {
    shaders[name] = Shader(vert, frag);
    return shaders[name];
}

// load shader from vertex, geometry and fragment shader files
Shader Resource_manager::load_shader(const std::string& vert,
        const std::string& geom, const std::string& frag,
        const std::string& name) {
    shaders[name] = Shader(vert, geom, frag);
    return shaders[name];
}

Shader Resource_manager::shader(const std::string& name) {
    return shaders[name];
}

Texture2D Resource_manager::texture(const std::string& name) {
    return textures[name];
}

Texture2D Resource_manager::load_texture(const std::string& img_fn,
        const GLboolean alpha, const std::string& name) {
    textures[name] = load_texture_file(img_fn, alpha);
    return textures[name];
}

Texture2D Resource_manager::load_texture_file(const std::string& img_fn,
        const GLboolean alpha) {
    Texture2D tex;
    if (alpha) {
        tex.internal_format = GL_RGBA;
        tex.image_format = GL_RGBA;
    }

    // loading image from file
    int img_w, img_h;
    unsigned char *img = SOIL_load_image(img_fn.c_str(), &img_w, &img_h, 0,
            alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

    // generate texture
    tex.generate(img_w, img_h, img);

    // free image data
    SOIL_free_image_data(img);

    return tex;
}

#endif /* RESOURCE_MANAGER_H */

