#pragma once

#include <world/world.hpp>
#include <world/block_registry.hpp>
#include <world/chunk_position.hpp>
#include <graphics/atlas_coord.hpp>
#include <graphics/vertex.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

struct Meshes {
    MeshData Opaque;
    MeshData Transparent;
};

enum class FaceDirection {
    Top, Bottom,
    Front, Back,
    Left, Right
};

class Mesher {
    public:
        static Meshes GenerateMesh(World& world, const ChunkPosition& chunkPosition);
    private:
        static void addFace(MeshData& mesh, World& world, glm::vec3 position, glm::ivec3 neighborPosition, FaceDirection direction, BlockType type);
        static void addCross(MeshData& mesh, World& world, glm::vec3 position, BlockType type);
        static FaceData getDataForFace(const BlockFaceTextures& textures, FaceDirection direction);
        static bool isOccluder(World& world, glm::ivec3 position);
        static bool shouldRenderFace(BlockType current, BlockType neighbor);
        static float calculateAO(World& world, glm::ivec3 side1, glm::ivec3 side2, glm::ivec3 corner);
};