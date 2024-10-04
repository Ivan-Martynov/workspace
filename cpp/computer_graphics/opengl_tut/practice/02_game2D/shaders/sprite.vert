#version 330 core

// <vec2 position, vec2 tex_coords>
layout (location = 0) in vec4 vert;

out vec2 tex_coords;

uniform mat4 model;
uniform mat4 proj;

void main() {
    tex_coords  = vert.zw;
    gl_Position = proj * model * vec4(vert.xy, 0, 1);
}

