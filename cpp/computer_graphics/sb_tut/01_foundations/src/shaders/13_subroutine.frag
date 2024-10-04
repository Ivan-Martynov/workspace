#version 450 core

subroutine vec4 sub_changeColor(vec4 param);

subroutine (sub_changeColor)
vec4 changeColor1(vec4 param) {
    return param * vec4(1, 0.25, 0.25, 1);
}

subroutine (sub_changeColor)
vec4 changeColor2(vec4 param) {
    return param * vec4(0.25, 0.25, 1, 1);
}

subroutine uniform sub_changeColor changeColorSubroutine;

out vec4 color;

void main() {
    color = changeColorSubroutine(vec4(1));
}

