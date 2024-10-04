#version 450 core

void main() {
    const vec4 vertices[] = vec4[](
            vec4(-1, -1, 0.5, 1),
            vec4( 1, -1, 0.5, 1),
            vec4(-1,  1, 0.5, 1),
            vec4( 1,  1, 0.5, 1));

    gl_Position = vertices[gl_VertexID];
}

