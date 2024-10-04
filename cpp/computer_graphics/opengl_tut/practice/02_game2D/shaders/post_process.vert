#version 330 core

// <vec2 position, vec2 tex_coords>
layout (location = 0) in vec4 vert;

out vec2 tex_coords;

uniform bool is_chaos;
uniform bool is_confuse;
uniform bool is_shake;
uniform float time_val;

void main() {
    gl_Position = vec4(vert.xy, 0, 1);
    vec2 tex = vert.zw;
    if (is_chaos) {
        float strength = 0.3;
        vec2 pos = vec2(tex.x + sin(time_val) * strength,
                tex.y + cos(time_val) * strength);
        tex_coords = pos;
    } else if (is_confuse) {
        tex_coords = vec2(1 - tex.x, 1 - tex.y);
    } else {
        tex_coords = tex;
    }
    if (is_shake) {
        float strength = 0.01;
        gl_Position.x += cos(time_val * 10) * strength;
        gl_Position.y += cos(time_val * 15) * strength;
    }
}

