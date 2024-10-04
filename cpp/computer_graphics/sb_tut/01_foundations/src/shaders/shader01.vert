#version 450 core

void main() {
    const vec4 vertices[] = vec4[] (
            vec4(0.75, -0.75, 0.5, 1),
            vec4(-0.75, -0.75, 0.5, 1),
            vec4(0.75, 0.75, 0.5, 1));

    gl_Position = vertices[gl_VertexID];
    //gl_Position = vec4(0, 0, 0.5, 1);
}

