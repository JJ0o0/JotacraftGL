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

enum class FaceDirection {
    Top, Bottom,
    Front, Back,
    Left, Right
};

class Mesher {
    public:
        static MeshData GenerateMesh(World& world, const ChunkPosition& chunkPosition);
    private:
        static void addFace(MeshData& mesh, glm::vec3 position, FaceDirection direction, BlockType type);
        static FaceData getDataForFace(const BlockFaceTextures& textures, FaceDirection direction);
};