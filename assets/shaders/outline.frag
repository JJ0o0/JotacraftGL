#version 410 core
out vec4 FragPos;
in vec3 LocalPos;

uniform vec3 uColor;
uniform float uThickness;

void main() {
    vec3 dist = min(LocalPos, 1.0 - LocalPos);

    bool nearEdgeX = dist.x < uThickness;
    bool nearEdgeY = dist.y < uThickness;
    bool nearEdgeZ = dist.z < uThickness;

    int nearCount = int(nearEdgeX) + int(nearEdgeY) + int(nearEdgeZ);
    if (nearCount < 2) discard;

    FragPos = vec4(uColor, 1.0);
}