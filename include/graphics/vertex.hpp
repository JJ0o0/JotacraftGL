#pragma once

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 Position{0.0f};
    glm::vec3 Normal{0.0f};
    glm::vec2 TexCoord{0.0f};
    glm::vec3 Color{1.0f};
    float AO = 1.0f;
    float SkyLight = 15.0f;
    float ReceivesDiffuse = 1.0f;
};

struct Vertex2D {
    glm::vec2 Position{0.0f};
    glm::vec2 TexCoord{0.0f};
    glm::vec4 Color{1.0f};
};