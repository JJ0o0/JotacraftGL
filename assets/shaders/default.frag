#version 410 core
out vec4 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Color;
in float AO;
in float SkyLight;
in float ReceivesDiffuse;

struct SunLight {
    vec3 Color;
    vec3 Direction;
};

uniform SunLight uSun;
uniform sampler2D uTexture;

void main() {
    float sky = SkyLight / 15.0;
    sky = max(sky, 0.05);

    vec3 ambient = vec3(0.4);
    vec3 sunDirection = normalize(uSun.Direction);
    float ao = mix(0.4, 1.0, AO / 3.0);

    float diffuseStrength = ReceivesDiffuse;
    float diffuse = max(dot(Normal, -sunDirection), 0.0);
    diffuse *= diffuseStrength;

    vec3 lighting = (ambient + diffuse * uSun.Color + (1.0 - diffuseStrength) * vec3(1.0)) * sky * ao;

    vec4 textureColor = texture(uTexture, TexCoord);

    if (textureColor.a < 0.1) discard;
    FragPos = vec4(textureColor.rgb * Color * lighting, textureColor.a);
}