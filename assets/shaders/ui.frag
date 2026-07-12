#version 410 core
out vec4 FragPos;
in vec2 TexCoord;
in vec4 Color;

uniform vec4 uTint;
uniform bool uHasTexture;
uniform sampler2D uTexture;

void main() {
    vec4 total = Color;

    if (uHasTexture) {
        total *= texture(uTexture, TexCoord); 
    }

    FragPos = total * uTint;
}