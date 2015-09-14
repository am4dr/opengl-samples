#version 330 core

layout(location=0) out vec4 color;
in float vfColor;
void main(void) {
   color = vec4(0.8f, 0.8f, vfColor, 1.0f);
}
