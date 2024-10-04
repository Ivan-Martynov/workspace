#version 450 core

layout (location = 0) out vec4 color;

in VS_OUT {
    flat int alien;
    vec2 tex_coord;
} fs_in;

//uniform sampler2DArray texture_aliens;
layout (binding = 0) uniform sampler2DArray texture_aliens;

void main() {
    //color = texture(texture_aliens, vec3(fs_in.tex_coord, float(fs_in.alien)));
    color = texture(texture_aliens, vec3(fs_in.tex_coord, fs_in.alien));
}

