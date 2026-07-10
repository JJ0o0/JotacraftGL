#pragma once

#include <graphics/mesh.hpp>
#include <graphics/vertex.hpp>
#include <vector>

inline Mesh CreateQuad() {
    glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f);

    std::vector<Vertex> vertices {
        { glm::vec3(-0.5f, 0.5f, 0.0f), normal, glm::vec2(0.0f, 1.0f)},
		{ glm::vec3(0.5f, 0.5f, 0.0f), normal, glm::vec2(1.0f, 1.0f)},
		{ glm::vec3(-0.5f, -0.5f, 0.0f), normal, glm::vec2(0.0f, 0.0f)},
		{ glm::vec3(0.5f, -0.5f, 0.0f), normal, glm::vec2(1.0f, 0.0f)},
    };

    std::vector<uint32_t> indices {
        0, 1, 2,
        1, 3, 2
    };

    return Mesh(vertices, indices);
}