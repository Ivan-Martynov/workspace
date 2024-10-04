#version 330 core

out vec4 frag_color;

in vec2 vert_tex;

uniform sampler2D depth_map;
uniform float near_plane;
uniform float far_plane;

void main() {
    float depth_value = texture(depth_map, vert_tex).r;
    frag_color = vec4(vec3(depth_value), 1);
}
