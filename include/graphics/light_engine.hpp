#pragma once

#include <world/world.hpp>
#include <world/chunk_position.hpp>
#include <glm/glm.hpp>
#include <queue>

struct LightNode {
    glm::ivec3 Position;
};

struct LightRemovalNode {
    glm::ivec3 Position;
    uint8_t Value;
};

class LightEngine {
public:
    static void InitializeSkyLight(World& world, const ChunkPosition& chunkPos);
    static void OnBlockPlaced(World& world, const glm::ivec3& position);
    static void OnBlockBroken(World& world, const glm::ivec3& position);
private:
    static void propagateSkyLight(World& world, std::queue<LightNode>& queue);
    static void removeSkyLight(World& world, std::queue<LightRemovalNode>& removalQueue, std::queue<LightNode>& relightQueue);
    static bool blocksLight(World& world, glm::ivec3 position);
};