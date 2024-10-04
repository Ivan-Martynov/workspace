#version 330 core

// <vec2 position, vec2 tex_coords>
layout (location = 0) in vec4 vert;

out vec2 tex_coords;
out vec4 particle_clr;

uniform mat4 proj;
uniform vec2 offset;
uniform vec4 color;

void main() {
    float scale  = 10;
    tex_coords   = vert.zw;
    particle_clr = color;
    gl_Position  = proj * vec4((vert.xy * scale) + offset, 0, 1);
}

