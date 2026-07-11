#pragma once

#include "chunk.hpp"
#include "chunk_position.hpp"
#include <unordered_map>

class World {
    public:
        void GenerateChunk(const ChunkPosition& position, int groundHeight);
        Chunk* GetChunk(const ChunkPosition& position);
        BlockType GetBlock(int x, int y, int z);
    private:
        std::unordered_map<ChunkPosition, Chunk> m_chunks{};

        ChunkPosition worldToChunkPosition(int x, int z);
};