#pragma once

#include <glm/glm.hpp>

struct SunLight {
    glm::vec3 Color{1.0f, 1.0f, 0.95f};
    glm::vec3 Direction{-0.5f, -0.7f, -0.3f};
};