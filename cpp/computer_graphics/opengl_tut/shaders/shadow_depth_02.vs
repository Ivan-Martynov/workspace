#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;

out vec2 vert_tex;

uniform mat4 model;

void main() {
    gl_Position = model * vec4(position, 1);
    vert_tex = tex_coord;
}
