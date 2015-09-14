#version 330 core

out vec4 fColor;

in vec4 vfColor;

void main() {
    fColor = vfColor;
}
