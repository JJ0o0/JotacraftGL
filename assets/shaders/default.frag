#version 410 core
out vec4 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D uTexture;

void main() {
    FragPos = texture(uTexture, TexCoord * 2.0);
}