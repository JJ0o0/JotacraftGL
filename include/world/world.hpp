#pragma once

#include <world/chunk.hpp>
#include <world/chunk_position.hpp>
#include <world/noise.hpp>
#include <glm/ext/vector_float3.hpp>
#include <unordered_map>

struct WorldUpdateResult {
    std::vector<ChunkPosition> Created;
    std::vector<ChunkPosition> Removed;
};

struct WorldSettings {
    float RenderDistance = 4;
};

class World {
    public:
        WorldUpdateResult Update(const glm::vec3& playerPosition);

        void GenerateChunk(const ChunkPosition& position);
        void RemoveChunk(const ChunkPosition& pos);
        void SetBlock(int x, int y, int z, BlockType type);
        void SetSkyLight(int x, int y, int z, uint8_t level);

        bool HasChunk(const ChunkPosition& pos) const;
        Chunk* GetChunk(const ChunkPosition& position);
        BlockType GetBlock(int x, int y, int z);
        uint8_t GetSkyLight(int x, int y, int z);
        int GetSurfaceHeight(int x, int z);
        
        ChunkPosition WorldToChunkPosition(int x, int z);

        const auto& GetChunks() const { return m_chunks; }
    private:
        std::unordered_map<ChunkPosition, Chunk> m_chunks{};

        WorldSettings m_settings{};
        Noise m_noise{};
};