#version 330 core

in vec2 tex_coords;

out vec4 color;

uniform sampler2D img;
uniform vec3 sprite_color;

void main() {
    color = vec4(sprite_color, 1) * texture(img, tex_coords);
}

