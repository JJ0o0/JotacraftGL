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
        static void addFace(MeshData& mesh, World& world, Chunk* chunk, const ChunkPosition& chunkPos, glm::ivec3 localPos, glm::ivec3 worldPos, glm::ivec3 neighborLocal, FaceDirection direction, BlockType type);
        static void addCross(MeshData& mesh, World& world, Chunk* chunk, const ChunkPosition& chunkPos, glm::ivec3 localPos, glm::ivec3 worldPos, BlockType type);
        static FaceData getDataForFace(const BlockFaceTextures& textures, FaceDirection direction);
        static BlockType sampleBlock(World& world, Chunk* chunk, const ChunkPosition& chunkPos, int lx, int ly, int lz);
        static uint8_t sampleSkyLight(World& world, Chunk* chunk, const ChunkPosition& chunkPos, int lx, int ly, int lz);
        static bool sampleOccluder(World& world, Chunk* chunk, const ChunkPosition& chunkPos, int lx, int ly, int lz);
        static bool shouldRenderFace(BlockType current, BlockType neighbor);
        static float calculateAO(World& world, Chunk* chunk, const ChunkPosition& chunkPos, glm::ivec3 side1, glm::ivec3 side2, glm::ivec3 corner);
};