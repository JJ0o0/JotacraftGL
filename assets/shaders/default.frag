#version 410 core
out vec4 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Color;

uniform sampler2D uTexture;

void main() {
    FragPos = texture(uTexture, TexCoord) * vec4(Color, 1.0);
}