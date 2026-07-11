#pragma once
#include <glm/glm.hpp>

struct AtlasCoord {
    int column = 0;
    int row = 0;

    static constexpr int ATLAS_SIZE = 16;

    glm::vec2 GetUVMin() const {
        float cellSize = 1.0f / ATLAS_SIZE;
        return {
            column * cellSize,
            1.0f - ((row + 1) * cellSize)
        };
    }

    glm::vec2 GetUVMax() const {
        float cellSize = 1.0f / ATLAS_SIZE;
        return {
            (column + 1) * cellSize,
            1.0f - (row * cellSize)
        };
    }
};