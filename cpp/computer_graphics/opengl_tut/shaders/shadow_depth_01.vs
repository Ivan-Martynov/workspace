#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 light_matrix;
uniform mat4 model;

void main() {
    gl_Position = light_matrix * model * vec4(position, 1);
}
