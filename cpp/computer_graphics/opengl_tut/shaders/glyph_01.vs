#version 330 core

layout (location = 0) in vec4 vert; // <vec2 pos, vec2 tex>

out vec2 tex_coords;

uniform mat4 proj;

void main() {
    gl_Position = proj * vec4(vert.xy, 0, 1);
    tex_coords  = vert.zw;
}

