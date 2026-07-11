#pragma once

#include <cstddef>
#include <functional>

struct ChunkPosition {
    int x = 0;
    int z = 0;

    bool operator==(const ChunkPosition& other) const {
        return other.x == x && other.z == z;
    }
};

namespace std {
    template<>
    struct hash<ChunkPosition> {
        size_t operator()(const ChunkPosition& position) const {
            return std::hash<int>{}(position.x) ^ (std::hash<int>{}(position.z) << 1);
        }
    };
}