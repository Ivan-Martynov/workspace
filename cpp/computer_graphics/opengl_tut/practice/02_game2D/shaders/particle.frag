#version 330 core

in vec2 tex_coords;
in vec4 particle_clr;

out vec4 color;

uniform sampler2D sprite_img;

void main() {
    color = texture(sprite_img, tex_coords) * particle_clr;
}

