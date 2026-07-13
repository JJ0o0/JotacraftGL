#include <graphics/light_engine.hpp>
#include <world/block_registry.hpp>

void LightEngine::InitializeSkyLight(World& world, const ChunkPosition& chunkPos) {
    std::queue<LightNode> queue;

    for (int x = 0; x < Chunk::CHUNK_SIZE; x++) {
        for (int z = 0; z < Chunk::CHUNK_SIZE; z++) {
            int worldX = chunkPos.x * Chunk::CHUNK_SIZE + x;
            int worldZ = chunkPos.z * Chunk::CHUNK_SIZE + z;

            for (int y = Chunk::CHUNK_SIZE - 1; y >= 0; y--) {
                if (blocksLight(world, {worldX, y, worldZ})) break;

                world.SetSkyLight(worldX, y, worldZ, 15);
                queue.push({glm::ivec3(worldX, y, worldZ)});
            }
        }
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
    for (int y = position.y + 1; y < Chunk::CHUNK_SIZE; y++) {
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
    queue.push({position});
    propagateSkyLight(world, queue);
}

void LightEngine::propagateSkyLight(World& world, std::queue<LightNode>& queue) {
    static const glm::ivec3 offsets[6] = {
        { 1, 0, 0}, {-1, 0, 0},
        { 0, 1, 0}, { 0,-1, 0},
        { 0, 0, 1}, { 0, 0,-1}
    };

    while (!queue.empty()) {
        LightNode node = queue.front();
        queue.pop();

        uint8_t currentLight = world.GetSkyLight(node.Position.x, node.Position.y, node.Position.z);

        for (const auto& offset : offsets) {
            glm::ivec3 neighborPos = node.Position + offset;
            if (blocksLight(world, neighborPos)) continue;

            uint8_t decay = (offset.y == -1 && currentLight == 15) ? 0 : 1;
            uint8_t newLight = currentLight > decay ? currentLight - decay : 0;

            uint8_t neighborLight = world.GetSkyLight(neighborPos.x, neighborPos.y, neighborPos.z);
            if (newLight > neighborLight) {
                world.SetSkyLight(neighborPos.x, neighborPos.y, neighborPos.z, newLight);
                queue.push({neighborPos});
            }
        }
    }
}

void LightEngine::removeSkyLight(World& world, std::queue<LightRemovalNode>& removalQueue, std::queue<LightNode>& relightQueue) {
    static const glm::ivec3 offsets[6] = {
        { 1, 0, 0}, {-1, 0, 0},
        { 0, 1, 0}, { 0,-1, 0},
        { 0, 0, 1}, { 0, 0,-1}
    };

    while (!removalQueue.empty()) {
        LightRemovalNode node = removalQueue.front();
        removalQueue.pop();

        for (const auto& offset : offsets) {
            glm::ivec3 neighborPos = node.Position + offset;
            if (blocksLight(world, neighborPos)) continue;

            uint8_t neighborLight = world.GetSkyLight(neighborPos.x, neighborPos.y, neighborPos.z);
            if (neighborLight == 0) continue;

            bool noDecay = (offset.y == -1 && node.Value == 15);
            bool causedByNode = noDecay ? (neighborLight == node.Value) : (neighborLight < node.Value);

            if (causedByNode) {
                world.SetSkyLight(neighborPos.x, neighborPos.y, neighborPos.z, 0);
                removalQueue.push({neighborPos, neighborLight});
            } else {
                relightQueue.push({neighborPos});
            }
        }
    }
}

bool LightEngine::blocksLight(World& world, glm::ivec3 position) {
    BlockType block = world.GetBlock(position.x, position.y, position.z);

    if (block == BlockType::Air) return false;
    return BlockRegistry::Get(block).BlocksLight;
}