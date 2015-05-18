#version 330 core

layout(location=0) in vec2 position;
layout(location=1) in float color;
out vec2 newPosition;
out float vfColor;
float step = 0.005f;
void main(void) {
   vec2 normal = normalize(position);
   vec2 pos = position + step * normal;
   if (pos.x > 1.0f || pos.x < -1.0f
       || pos.y > 1.0f || pos.y < -1.0f) {
       pos = step * normalize(pos);
   }
   newPosition = pos;
   gl_Position = vec4(position, 0.0f, 1.0f);
   vfColor = color;
}
