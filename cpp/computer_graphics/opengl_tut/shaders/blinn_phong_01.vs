#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coords;

out VS_OUT {
    vec3 frag_pos;
    vec3 norm_vec;
    vec2 vert_tex;
} vs_out;

uniform mat4 proj;
uniform mat4 view;

void main() {
    gl_Position = proj * view * vec4(position, 1);
    vs_out.frag_pos = position;
    vs_out.norm_vec = normal;
    vs_out.vert_tex = tex_coords;
}
