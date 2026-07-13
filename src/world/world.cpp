#include <world/world.hpp>
#include <world/block_registry.hpp>
#include <math.hpp>
#include <cmath>
#include <chrono>
#include <iostream>
#include <algorithm>

WorldUpdateResult World::Update(const glm::vec3& playerPosition) {
    auto start = std::chrono::high_resolution_clock::now();

    ChunkPosition playerChunk = WorldToChunkPosition(
        static_cast<int>(std::floor(playerPosition.x)),
        static_cast<int>(std::floor(playerPosition.z))
    );

    WorldUpdateResult result;

    std::vector<ChunkPosition> missing;
    for (int dx = -m_settings.RenderDistance; dx <= m_settings.RenderDistance; dx++) {
        for (int dz = -m_settings.RenderDistance; dz <= m_settings.RenderDistance; dz++) {
            ChunkPosition pos{
                playerChunk.x + dx,
                playerChunk.z + dz
            };

            if (HasChunk(pos)) continue;

            missing.push_back(pos);
        }
    }

    std::sort(missing.begin(), missing.end(), [&](const ChunkPosition& a, const ChunkPosition& b) {
        int adx = a.x - playerChunk.x;
        int adz = a.z - playerChunk.z;
        int bdx = b.x - playerChunk.x;
        int bdz = b.z - playerChunk.z;

        int distA = adx * adx + adz * adz;
        int distB = bdx * bdx + bdz * bdz;

        return distA < distB;
    });

    for (const ChunkPosition& pos : missing) {
        GenerateChunk(pos);
        result.Created.push_back(pos);
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout
        << "World Update: "
        << std::chrono::duration<float, std::milli>(end - start).count()
        << " ms\n";

    return result;
}

void World::GenerateChunk(const ChunkPosition& position) {  
    auto start = std::chrono::high_resolution_clock::now();
  
    Chunk& chunk = m_chunks[position];
    chunk.GenerateTerrain(m_noise, position.x, position.z);

    auto end = std::chrono::high_resolution_clock::now();

    std::cout
        << "GenerateChunk: "
        << std::chrono::duration<float, std::milli>(end-start).count()
        << " ms\n";
}

void World::RemoveChunk(const ChunkPosition& pos) { m_chunks.erase(pos); }

void World::SetBlock(int x, int y, int z, BlockType type) {
    ChunkPosition chunkPosition = WorldToChunkPosition(x, z);
    Chunk* chunk = GetChunk(chunkPosition);
    if (!chunk) return;

    int localX = x - (chunkPosition.x * Chunk::CHUNK_SIZE_X);
    int localY = y;
    int localZ = z - (chunkPosition.z * Chunk::CHUNK_SIZE_Z);

    chunk->SetVoxel(localX, localY, localZ, type);
}

void World::SetSkyLight(int x, int y, int z, uint8_t level) {
    ChunkPosition chunkPosition = WorldToChunkPosition(x, z);
    Chunk* chunk = GetChunk(chunkPosition);
    if (!chunk) return;

    int localX = x - (chunkPosition.x * Chunk::CHUNK_SIZE_X);
    int localY = y;
    int localZ = z - (chunkPosition.z * Chunk::CHUNK_SIZE_Z);

    chunk->SetSkyLight(localX, localY, localZ, level);
}

bool World::HasChunk(const ChunkPosition& pos) const { return m_chunks.contains(pos); }

Chunk* World::GetChunk(const ChunkPosition& position) {
    auto it = m_chunks.find(position);
    if (it == m_chunks.end()) return nullptr;
    return &it->second;
}

BlockType World::GetBlock(int x, int y, int z) {
    ChunkPosition chunkPosition = WorldToChunkPosition(x, z);
    Chunk* chunk = GetChunk(chunkPosition);
    if (!chunk) return BlockType::Air;

    int localX = x - (chunkPosition.x * Chunk::CHUNK_SIZE_X);
    int localY = y;
    int localZ = z - (chunkPosition.z * Chunk::CHUNK_SIZE_Z);

    return chunk->GetVoxel(localX, localY, localZ);
}

uint8_t World::GetSkyLight(int x, int y, int z) {
    ChunkPosition chunkPosition = WorldToChunkPosition(x, z);
    Chunk* chunk = GetChunk(chunkPosition);
    if (!chunk) return 0;

    int localX = x - (chunkPosition.x * Chunk::CHUNK_SIZE_X);
    int localY = y;
    int localZ = z - (chunkPosition.z * Chunk::CHUNK_SIZE_Z);

    return chunk->GetSkyLight(localX, localY, localZ);
}

int World::GetSurfaceHeight(int x, int z) {
    for (int y = Chunk::CHUNK_SIZE_Y - 1; y > 0; y--) {
        BlockType block = GetBlock(x, y, z);
        if (!BlockRegistry::Get(block).Solid) continue;

        return y + 1;
    }

    return 0;
}

ChunkPosition World::WorldToChunkPosition(int x, int z) {
    return ChunkPosition{
        FloorDivision(x, Chunk::CHUNK_SIZE_X),
        FloorDivision(z, Chunk::CHUNK_SIZE_Z)
    };
}