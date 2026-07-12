#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColor;

out vec2 TexCoord;
out vec4 Color;

uniform vec2 uUvMin;
uniform vec2 uUvMax;
uniform mat4 uModel;
uniform mat4 uProjection;

void main() {
    gl_Position = uProjection * uModel * vec4(aPos, 1.0);
    
    TexCoord = uUvMin + aTexCoord * (uUvMax - uUvMin);
    Color = aColor;
}