# version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 instancePosition;
layout(location = 2) in vec3 instanceColor;

out vec4 vColor;

void main(void) {
    gl_Position = vec4((position / 10.0) + instancePosition, 1.0f);
    //gl_Position = vec4(position, 1.0f);
    // gl_Position = vec4(instancePosition, 1.0f);
    vColor = vec4(instanceColor, 1.0f);
}
