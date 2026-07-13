#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;
layout (location = 4) in float aAO;
layout (location = 5) in float aSkyLight;
layout (location = 6) in float aReceivesDiffuse;

out vec3 Normal;
out vec2 TexCoord;
out vec3 Color;
out float AO;
out float SkyLight;
out float ReceivesDiffuse;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    Normal = normalize(transpose(inverse(mat3(uModel))) * aNormal);
    TexCoord = aTexCoord;
    Color = aColor;
    AO = aAO;
    SkyLight = aSkyLight;
    ReceivesDiffuse = aReceivesDiffuse;
}