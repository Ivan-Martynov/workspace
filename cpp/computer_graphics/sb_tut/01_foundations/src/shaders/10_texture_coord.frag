#version 450 core

uniform sampler2D texture_object;

in vec2 tex_coords;

out vec4 color;

void main() {
    color = texture(texture_object, tex_coords);
}

