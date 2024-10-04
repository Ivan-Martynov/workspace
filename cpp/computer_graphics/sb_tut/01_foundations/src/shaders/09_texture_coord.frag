#version 450 core

in VS_OUT {
    vec2 tex_coord;
} vs_in;

layout (location = 0) out vec4 color;
layout (binding = 0) uniform sampler2D texture_object;

void main() {
    color = texture(texture_object, vs_in.tex_coord);
}

