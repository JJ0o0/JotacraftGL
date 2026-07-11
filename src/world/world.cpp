#include <world/world.hpp>
#include <math.hpp>

void World::GenerateChunk(const ChunkPosition& position, int groundHeight) {
    Chunk& chunk = m_chunks[position];
    chunk.GenerateFlat(groundHeight);
}

Chunk* World::GetChunk(const ChunkPosition& position) {
    auto it = m_chunks.find(position);
    if (it == m_chunks.end()) return nullptr;
    return &it->second;
}

BlockType World::GetBlock(int x, int y, int z) {
    ChunkPosition chunkPosition = worldToChunkPosition(x, z);
    Chunk* chunk = GetChunk(chunkPosition);
    if (!chunk) return BlockType::Air;

    int localX = x - (chunkPosition.x * Chunk::CHUNK_SIZE);
    int localY = y;
    int localZ = z - (chunkPosition.z * Chunk::CHUNK_SIZE);

    return chunk->GetVoxel(localX, localY, localZ);
}

ChunkPosition World::worldToChunkPosition(int x, int z) {
    return ChunkPosition{
        FloorDivision(x, Chunk::CHUNK_SIZE),
        FloorDivision(z, Chunk::CHUNK_SIZE)
    };
}