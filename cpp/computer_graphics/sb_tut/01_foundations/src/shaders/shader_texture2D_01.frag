#version 450 core

uniform sampler2D s;

out vec4 color;

void main() {
    color = texture(s, gl_FragCoord.xy / textureSize(s, 0));
    //color = texelFetch(s, ivec2(gl_FragCoord.xy), 0);
}

