#version 450 core

//layout (location = 0) in vec4 offset;
//layout (location = 1) in vec4 color;

//out VS_OUT
//{
//    vec4 color;
//} vs_out;

//out vec4 vs_color;

void main() {
    const vec4 vertices[3] = vec4[3] (
            vec4(0.25, -0.25, 0.5, 1),
            vec4(-0.25, -0.25, 0.5, 1),
            vec4(0.25, 0.25, 0.5, 1));

    gl_Position = vertices[gl_VertexID];

    //vs_out.color = color;
    //vs_color = color;
}

