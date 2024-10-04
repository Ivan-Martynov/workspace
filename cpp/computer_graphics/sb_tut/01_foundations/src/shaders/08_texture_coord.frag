#version 450 core

layout (binding = 0) uniform sampler2D tex_object;

in VS_OUT {
    vec2 tex_coord;
} vs_in;

out vec4 color;

void main() {
    color = texture(tex_object, vs_in.tex_coord * vec2(3, 1));
}

