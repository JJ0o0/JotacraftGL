#include <graphics/light_engine.hpp>
#include <iostream>
#include <world/block_registry.hpp>

void LightEngine::InitializeSkyLight(World& world, const std::vector<ChunkPosition>& chunks){
    std::queue<LightNode> queue;

    for (const auto& chunkPos : chunks) {
        for (int x = 0; x < Chunk::CHUNK_SIZE_X; x++) {
            for (int z = 0; z < Chunk::CHUNK_SIZE_Z; z++) {
                int worldX = chunkPos.x * Chunk::CHUNK_SIZE_X + x;
                int worldZ = chunkPos.z * Chunk::CHUNK_SIZE_Z + z;

                int topY = Chunk::CHUNK_SIZE_Y - 1;

                for (; topY >= 0; topY--) {
                    if (blocksLight(world, {worldX, topY, worldZ})) break;                    
                    world.SetSkyLight(worldX, topY, worldZ, 15);

                    queue.push({
                        {worldX, topY, worldZ},
                        15
                    });
                }
            }
        }

        world.GetChunk(chunkPos)->MarkLightInitialized();
    }  

    propagateSkyLight(world, queue);
}

void LightEngine::OnBlockPlaced(World& world, const glm::ivec3& position) {
    uint8_t oldLight = world.GetSkyLight(position.x, position.y, position.z);
    world.SetSkyLight(position.x, position.y, position.z, 0);

    std::queue<LightRemovalNode> removalQueue;
    std::queue<LightNode> relightQueue;
    removalQueue.push({position, oldLight});

    removeSkyLight(world, removalQueue, relightQueue);
    propagateSkyLight(world, relightQueue);
}

void LightEngine::OnBlockBroken(World& world, const glm::ivec3& position) {
    bool exposedToSky = true;
    for (int y = position.y + 1; y < Chunk::CHUNK_SIZE_Y; y++) {
        if (blocksLight(world, {position.x, y, position.z})) {
            exposedToSky = false;
            break;
        }
    }

    uint8_t newLight = 0;
    if (exposedToSky) newLight = 15;
    else {
        static const glm::ivec3 offsets[6] = {
            { 1, 0, 0}, 
            {-1, 0, 0}, 
            { 0, 1, 0}, 
            { 0,-1, 0}, 
            { 0, 0, 1}, 
            { 0, 0,-1}
        };
        
        for (const auto& offset : offsets) {
            glm::ivec3 n = position + offset;
            uint8_t l = world.GetSkyLight(n.x, n.y, n.z);
            newLight = std::max(newLight, static_cast<uint8_t>(l > 0 ? l - 1 : 0));
        }
    }

    world.SetSkyLight(position.x, position.y, position.z, newLight);

    std::queue<LightNode> queue;
    queue.push({position, newLight});
    propagateSkyLight(world, queue);
}

void LightEngine::propagateSkyLight(World& world, std::queue<LightNode>& queue) {
    size_t processed = 0;
    static const glm::ivec3 offsets[6] = {
        { 1, 0, 0}, {-1, 0, 0},
        { 0, 1, 0}, { 0,-1, 0},
        { 0, 0, 1}, { 0, 0,-1}
    };

    ChunkPosition cachedPos{INT32_MIN, INT32_MIN};
    Chunk* cachedChunk = nullptr;

    while (!queue.empty()) {
        LightNode node = queue.front();
        queue.pop();

        processed++;
        if (processed > 5000000) throw std::runtime_error("SkyLight propagation overflow");

        ChunkPosition nodeChunkPos = world.WorldToChunkPosition(node.Position.x, node.Position.z);
        if (nodeChunkPos.x != cachedPos.x || nodeChunkPos.z != cachedPos.z) {
            cachedChunk = world.GetChunk(nodeChunkPos);
            cachedPos = nodeChunkPos;
        }
        if (!cachedChunk) continue;

        int localX = node.Position.x - nodeChunkPos.x * Chunk::CHUNK_SIZE_X;
        int localZ = node.Position.z - nodeChunkPos.z * Chunk::CHUNK_SIZE_Z;
        int localY = node.Position.y;

        uint8_t currentLight = node.Value;

        for (const auto& offset : offsets) {
            int ny = localY + offset.y;
            if (ny < 0 || ny >= static_cast<int>(Chunk::CHUNK_SIZE_Y)) continue;

            int nx = localX + offset.x;
            int nz = localZ + offset.z;
            bool local = nx >= 0 && nx < static_cast<int>(Chunk::CHUNK_SIZE_X) &&
                         nz >= 0 && nz < static_cast<int>(Chunk::CHUNK_SIZE_Z);

            glm::ivec3 neighborGlobal = node.Position + offset;
            Chunk* neighborChunk;
            int lx, lz;

            if (local) {
                neighborChunk = cachedChunk;
                lx = nx; lz = nz;
            } else {
                ChunkPosition neighborChunkPos = world.WorldToChunkPosition(neighborGlobal.x, neighborGlobal.z);
                neighborChunk = world.GetChunk(neighborChunkPos);
                if (!neighborChunk || !neighborChunk->IsLightInitialized()) continue;
                lx = neighborGlobal.x - neighborChunkPos.x * Chunk::CHUNK_SIZE_X;
                lz = neighborGlobal.z - neighborChunkPos.z * Chunk::CHUNK_SIZE_Z;
            }

            if (blocksLightLocal(neighborChunk, lx, ny, lz)) continue;

            uint8_t decay = (offset.y == -1 && currentLight == 15) ? 0 : 1;
            uint8_t newLight = currentLight > decay ? currentLight - decay : 0;

            uint8_t neighborLight = neighborChunk->GetSkyLight(lx, ny, lz);
            if (newLight > neighborLight) {
                neighborChunk->SetSkyLight(lx, ny, lz, newLight);
                queue.push({neighborGlobal, newLight});
            }
        }
    }

    std::cout << "Light nodes processed: " << processed << "\n";
}

void LightEngine::removeSkyLight(World& world, std::queue<LightRemovalNode>& removalQueue, std::queue<LightNode>& relightQueue) {
    static const glm::ivec3 offsets[6] = {
        { 1, 0, 0}, {-1, 0, 0},
        { 0, 1, 0}, { 0,-1, 0},
        { 0, 0, 1}, { 0, 0,-1}
    };

    ChunkPosition cachedPos{INT32_MIN, INT32_MIN};
    Chunk* cachedChunk = nullptr;

    while (!removalQueue.empty()) {
        LightRemovalNode node = removalQueue.front();
        removalQueue.pop();

        ChunkPosition nodeChunkPos = world.WorldToChunkPosition(node.Position.x, node.Position.z);
        if (nodeChunkPos.x != cachedPos.x || nodeChunkPos.z != cachedPos.z) {
            cachedChunk = world.GetChunk(nodeChunkPos);
            cachedPos = nodeChunkPos;
        }
        if (!cachedChunk) continue;

        int localX = node.Position.x - nodeChunkPos.x * Chunk::CHUNK_SIZE_X;
        int localZ = node.Position.z - nodeChunkPos.z * Chunk::CHUNK_SIZE_Z;
        int localY = node.Position.y;

        for (const auto& offset : offsets) {
            int ny = localY + offset.y;
            if (ny < 0 || ny >= static_cast<int>(Chunk::CHUNK_SIZE_Y)) continue;

            int nx = localX + offset.x;
            int nz = localZ + offset.z;
            bool local = nx >= 0 && nx < static_cast<int>(Chunk::CHUNK_SIZE_X) &&
                         nz >= 0 && nz < static_cast<int>(Chunk::CHUNK_SIZE_Z);

            glm::ivec3 neighborGlobal = node.Position + offset;
            Chunk* neighborChunk;
            int lx, lz;

            if (local) {
                neighborChunk = cachedChunk;
                lx = nx; lz = nz;
            } else {
                ChunkPosition neighborChunkPos = world.WorldToChunkPosition(neighborGlobal.x, neighborGlobal.z);
                neighborChunk = world.GetChunk(neighborChunkPos);
                if (!neighborChunk) continue;
                lx = neighborGlobal.x - neighborChunkPos.x * Chunk::CHUNK_SIZE_X;
                lz = neighborGlobal.z - neighborChunkPos.z * Chunk::CHUNK_SIZE_Z;
            }

            if (blocksLightLocal(neighborChunk, lx, ny, lz)) continue;

            uint8_t neighborLight = neighborChunk->GetSkyLight(lx, ny, lz);
            if (neighborLight == 0) continue;

            bool noDecay = (offset.y == -1 && node.Value == 15);
            bool causedByNode = noDecay ? (neighborLight == node.Value) : (neighborLight < node.Value);

            if (causedByNode) {
                neighborChunk->SetSkyLight(lx, ny, lz, 0);
                removalQueue.push({neighborGlobal, neighborLight});
            } else {
                relightQueue.push({neighborGlobal, neighborLight});
            }
        }
    }
}

bool LightEngine::blocksLight(World& world, glm::ivec3 position) {
    BlockType block = world.GetBlock(position.x, position.y, position.z);

    if (block == BlockType::Air) return false;
    return BlockRegistry::Get(block).BlocksLight;
}

bool LightEngine::blocksLightLocal(Chunk* chunk, int x, int y, int z) {
    BlockType block = chunk->GetVoxel(x, y, z);
    if (block == BlockType::Air) return false;
    return BlockRegistry::Get(block).BlocksLight;
}