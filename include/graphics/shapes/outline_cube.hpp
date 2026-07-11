#pragma once

#include <graphics/mesh.hpp>
#include <graphics/vertex.hpp>
#include <vector>

inline Mesh CreateWireframeCube() {
    std::vector<Vertex> vertices {
        { glm::vec3(0, 0, 0) },
        { glm::vec3(1, 0, 0) },
        { glm::vec3(1, 1, 0) },
        { glm::vec3(0, 1, 1) },
        { glm::vec3(1, 0, 1) },
        { glm::vec3(1, 1, 1) },
        { glm::vec3(0, 1, 0) },
        { glm::vec3(0, 0, 1) },
    };

    std::vector<uint32_t> indices {
        0,1,4,  0,4,7,
        6,2,5,  6,5,3,
        0,6,2,  0,2,1,
        7,4,5,  7,5,3,
        0,7,3,  0,3,6,
        1,2,5,  1,5,4,
    };

    return Mesh(vertices, indices);
}