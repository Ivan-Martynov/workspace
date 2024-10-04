#version 450 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec4 color_in;

out vec4 vs_color;

uniform mat4 model_matrix;
uniform mat4 projection_matrix;

void main() {
    vs_color = color_in;
    gl_Position = projection_matrix * model_matrix * vec4(position, 0, 1);
}

