#version 410 core
out vec4 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Color;
in float AO;
in float SkyLight;

struct SunLight {
    vec3 Color;
    vec3 Direction;
};

uniform SunLight uSun;
uniform sampler2D uTexture;

void main() {
    float sky = SkyLight / 15.0;
    sky = max(sky, 0.05);

    vec3 ambient = vec3(0.25);

    vec3 sunDirection = normalize(uSun.Direction);
    float diffuse = max(dot(Normal, -sunDirection), 0.0);

    float ao = mix(0.4, 1.0, AO / 3.0);

    vec3 lighting = (ambient + diffuse * uSun.Color) * sky * ao;
    vec4 textureColor = texture(uTexture, TexCoord);

    FragPos = vec4(textureColor.rgb * Color * lighting, textureColor.a);
}