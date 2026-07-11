#include <world/world.hpp>
#include <math.hpp>
#include <cmath>

WorldUpdateResult World::Update(const glm::vec3& playerPosition) {
    ChunkPosition playerChunk = WorldToChunkPosition(
        static_cast<int>(std::floor(playerPosition.x)),
        static_cast<int>(std::floor(playerPosition.z))
    );

    WorldUpdateResult result;

    for (int dx = -m_settings.RenderDistance; dx <= m_settings.RenderDistance; dx++) {
        for (int dz = -m_settings.RenderDistance; dz <= m_settings.RenderDistance; dz++) {
            ChunkPosition pos{
                playerChunk.x + dx,
                playerChunk.z + dz
            };

            if (HasChunk(pos)) continue;

            GenerateChunk(pos, 6);
            result.Created.push_back(pos);
        }
    }

    for (auto it = m_chunks.begin(); it != m_chunks.end();) {
        int dx = std::abs(it->first.x - playerChunk.x);
        int dz = std::abs(it->first.z - playerChunk.z);

        if (dx > m_settings.RenderDistance + 1 || dz > m_settings.RenderDistance + 1) {
            result.Removed.push_back(it->first);
            it = m_chunks.erase(it);
        } else {
            ++it;
        }
    }

    return result;
}

void World::GenerateChunk(const ChunkPosition& position, int groundHeight) {    
    Chunk& chunk = m_chunks[position];
    chunk.GenerateFlat(groundHeight);
}

void World::RemoveChunk(const ChunkPosition& pos) { m_chunks.erase(pos); }

void World::SetBlock(int x, int y, int z, BlockType type) {
    ChunkPosition chunkPosition = WorldToChunkPosition(x, z);
    Chunk* chunk = GetChunk(chunkPosition);
    if (!chunk) return;

    int localX = x - (chunkPosition.x * Chunk::CHUNK_SIZE);
    int localY = y;
    int localZ = z - (chunkPosition.z * Chunk::CHUNK_SIZE);

    chunk->SetVoxel(localX, localY, localZ, type);
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

    int localX = x - (chunkPosition.x * Chunk::CHUNK_SIZE);
    int localY = y;
    int localZ = z - (chunkPosition.z * Chunk::CHUNK_SIZE);

    return chunk->GetVoxel(localX, localY, localZ);
}

ChunkPosition World::WorldToChunkPosition(int x, int z) {
    return ChunkPosition{
        FloorDivision(x, Chunk::CHUNK_SIZE),
        FloorDivision(z, Chunk::CHUNK_SIZE)
    };
}