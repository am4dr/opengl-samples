# version 330
uniform sampler2D sampler;

in vec2 vTexCoord;

out vec4 fColor;

void main(void) {
    fColor = texture(sampler, vTexCoord);
}

