#version 450 core

in VS_OUT
{
    vec4 color;
} vs_in;

out vec4 color;

void main() {
    color = vs_in.color;
}

