#version 450 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec4 color_in;

out vec4 vs_color;

uniform mat4 projection_matrix;

void main() {
    vs_color = color_in;
    //vs_color = vec4(color_in.r, position.xy, 1);
    //vs_color = vec4(color_in.r, position.x / 800, position.y / 600, 1);

    //gl_Position = vec4(position.xy, 0.5, 1);

    gl_Position = projection_matrix * vec4(position.xy, 0.5, 1);
}

