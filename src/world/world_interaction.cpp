#include <world/world_interaction.hpp>
#include <graphics/light_engine.hpp>

void WorldInteraction::PlaceBlock(World& world, Player& player, WorldRenderer& renderer, const RaycastResult& target, BlockType type) {
    if (!target.Hit) return;

    glm::ivec3 pos = target.PreviousVoxelPosition;

    AABB blockBox {
        glm::vec3(pos),
        glm::vec3(pos) + glm::vec3(1.0f)
    };

    if (player.GetAABB().Intersects(blockBox)) return;

    world.SetBlock(pos.x, pos.y, pos.z, type);

    ChunkPosition chunkPos = world.WorldToChunkPosition(pos.x, pos.z);
    LightEngine::OnBlockPlaced(world, pos);
    renderer.RegenerateChunk(world, chunkPos);

    int localX = pos.x - (chunkPos.x * Chunk::CHUNK_SIZE);
    int localZ = pos.z - (chunkPos.z * Chunk::CHUNK_SIZE);

    if (localX == 0) renderer.RegenerateChunk(world, {chunkPos.x - 1, chunkPos.z});
    if (localX == Chunk::CHUNK_SIZE - 1) renderer.RegenerateChunk(world, {chunkPos.x + 1, chunkPos.z});
    if (localZ == 0) renderer.RegenerateChunk(world, {chunkPos.x, chunkPos.z - 1});
    if (localZ == Chunk::CHUNK_SIZE - 1) renderer.RegenerateChunk(world, {chunkPos.x, chunkPos.z + 1});
}

void WorldInteraction::BreakBlock(World& world, WorldRenderer& renderer, const RaycastResult& target) {
    if (!target.Hit) return;

    glm::ivec3 pos = target.VoxelPosition;
    world.SetBlock(pos.x, pos.y, pos.z, BlockType::Air);
    
    ChunkPosition chunkPos = world.WorldToChunkPosition(pos.x, pos.z);
    LightEngine::OnBlockBroken(world, pos);
    renderer.RegenerateChunk(world, chunkPos);

    int localX = pos.x - (chunkPos.x * Chunk::CHUNK_SIZE);
    int localZ = pos.z - (chunkPos.z * Chunk::CHUNK_SIZE);

    if (localX == 0) renderer.RegenerateChunk(world, {chunkPos.x - 1, chunkPos.z});
    if (localX == Chunk::CHUNK_SIZE - 1) renderer.RegenerateChunk(world, {chunkPos.x + 1, chunkPos.z});
    if (localZ == 0) renderer.RegenerateChunk(world, {chunkPos.x, chunkPos.z - 1});
    if (localZ == Chunk::CHUNK_SIZE - 1) renderer.RegenerateChunk(world, {chunkPos.x, chunkPos.z + 1});
}
