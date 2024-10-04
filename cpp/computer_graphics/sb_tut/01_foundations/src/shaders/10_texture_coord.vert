#version 450 core

uniform vec2 offset;

out vec2 tex_coords;

void main() {

    const vec4[4] vertices = vec4[4](
            vec4(-0.45, -0.45, 0.5, 1),
            vec4( 0.45, -0.45, 0.5, 1),
            vec4(-0.45,  0.45, 0.5, 1),
            vec4( 0.45,  0.45, 0.5, 1));

    tex_coords = vertices[gl_VertexID].xy * 3 + vec2(0.45 * 3);

    gl_Position = vertices[gl_VertexID] + vec4(offset, 0, 0);
}

