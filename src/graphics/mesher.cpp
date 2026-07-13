#include <graphics/mesher.hpp>
#include <world/block_registry.hpp>

BlockType Mesher::sampleBlock(World& world, Chunk* chunk, const ChunkPosition& chunkPos, int lx, int ly, int lz) {
    if (lx >= 0 && lx < static_cast<int>(Chunk::CHUNK_SIZE_X) &&
        lz >= 0 && lz < static_cast<int>(Chunk::CHUNK_SIZE_Z)) {
        return chunk->GetVoxel(lx, ly, lz); // local, sem hash -- o próprio Chunk já trata ly fora dos limites
    }

    int worldX = chunkPos.x * Chunk::CHUNK_SIZE_X + lx;
    int worldZ = chunkPos.z * Chunk::CHUNK_SIZE_Z + lz;
    return world.GetBlock(worldX, ly, worldZ); // cruzou borda horizontal, precisa do World mesmo
}

uint8_t Mesher::sampleSkyLight(World& world, Chunk* chunk, const ChunkPosition& chunkPos, int lx, int ly, int lz) {
    if (lx >= 0 && lx < static_cast<int>(Chunk::CHUNK_SIZE_X) &&
        lz >= 0 && lz < static_cast<int>(Chunk::CHUNK_SIZE_Z)) {
        return chunk->GetSkyLight(lx, ly, lz);
    }

    int worldX = chunkPos.x * Chunk::CHUNK_SIZE_X + lx;
    int worldZ = chunkPos.z * Chunk::CHUNK_SIZE_Z + lz;
    return world.GetSkyLight(worldX, ly, worldZ);
}

bool Mesher::sampleOccluder(World& world, Chunk* chunk, const ChunkPosition& chunkPos, int lx, int ly, int lz) {
    BlockType block = sampleBlock(world, chunk, chunkPos, lx, ly, lz);
    if (block == BlockType::Air) return false;
    return BlockRegistry::Get(block).BlocksLight;
}

Meshes Mesher::GenerateMesh(World& world, const ChunkPosition& chunkPosition) {
    Meshes meshes;

    Chunk* chunk = world.GetChunk(chunkPosition);
    if (!chunk) return meshes;

    meshes.Opaque.vertices.reserve(20000);
    meshes.Opaque.indices.reserve(30000);
    meshes.Transparent.vertices.reserve(20000);
    meshes.Transparent.indices.reserve(30000);

    for (int x = 0; x < static_cast<int>(Chunk::CHUNK_SIZE_X); x++) {
        for (int y = 0; y < static_cast<int>(Chunk::CHUNK_SIZE_Y); y++) {
            for (int z = 0; z < static_cast<int>(Chunk::CHUNK_SIZE_Z); z++) {
                BlockType block = chunk->GetVoxel(x, y, z); // sempre local aqui, é o próprio voxel do loop
                if (block == BlockType::Air) continue;

                glm::ivec3 localPos{x, y, z};
                glm::ivec3 worldPos{
                    chunkPosition.x * static_cast<int>(Chunk::CHUNK_SIZE_X) + x,
                    y,
                    chunkPosition.z * static_cast<int>(Chunk::CHUNK_SIZE_Z) + z
                };

                const BlockData& data = BlockRegistry::Get(block);
                MeshData* mesh = nullptr;

                switch (data.Render) {
                    case RenderType::Opaque:
                        mesh = &meshes.Opaque;
                        break;
                    case RenderType::Transparent:
                        mesh = &meshes.Transparent;
                        break;
                    case RenderType::Cross:
                        addCross(meshes.Opaque, world, chunk, chunkPosition, localPos, worldPos, block);
                        continue;
                }

                static const glm::ivec3 offsets[6] = {
                    { 0, 1, 0}, { 0,-1, 0},
                    { 0, 0, 1}, { 0, 0,-1},
                    { 1, 0, 0}, {-1, 0, 0}
                };
                static const FaceDirection dirs[6] = {
                    FaceDirection::Top, FaceDirection::Bottom,
                    FaceDirection::Front, FaceDirection::Back,
                    FaceDirection::Right, FaceDirection::Left
                };

                for (int i = 0; i < 6; i++) {
                    glm::ivec3 nLocal = localPos + offsets[i];
                    BlockType neighbor = sampleBlock(world, chunk, chunkPosition, nLocal.x, nLocal.y, nLocal.z);

                    if (shouldRenderFace(block, neighbor)) {
                        addFace(*mesh, world, chunk, chunkPosition, localPos, worldPos, nLocal, dirs[i], block);
                    }
                }
            }
        }
    }

    return meshes;
}

void Mesher::addFace(MeshData& mesh, World& world, Chunk* chunk, const ChunkPosition& chunkPos, glm::ivec3 localPos, glm::ivec3 worldPos, glm::ivec3 neighborLocal, FaceDirection direction, BlockType type) {
    uint32_t baseIndex = static_cast<uint32_t>(mesh.vertices.size());
    const BlockData& data = BlockRegistry::Get(type);
    const BlockFaceTextures& textures = data.Textures;

    uint8_t rawLight = sampleSkyLight(world, chunk, chunkPos, neighborLocal.x, neighborLocal.y, neighborLocal.z);
    float skyLight = static_cast<float>(rawLight);

    AtlasCoord atlasCoord = getDataForFace(textures, direction).Texture;
    glm::vec2 uvMin = atlasCoord.GetUVMin();
    glm::vec2 uvMax = atlasCoord.GetUVMax();

    int lx = localPos.x, ly = localPos.y, lz = localPos.z;
    float px = static_cast<float>(worldPos.x), py = static_cast<float>(worldPos.y), pz = static_cast<float>(worldPos.z);

    switch (direction) {
        case FaceDirection::Top: {
            glm::vec3 normal{0, 1, 0};

            float ao0 = calculateAO(world, chunk, chunkPos, {lx-1, ly+1, lz}, {lx, ly+1, lz-1}, {lx-1, ly+1, lz-1});
            float ao1 = calculateAO(world, chunk, chunkPos, {lx-1, ly+1, lz}, {lx, ly+1, lz+1}, {lx-1, ly+1, lz+1});
            float ao2 = calculateAO(world, chunk, chunkPos, {lx+1, ly+1, lz}, {lx, ly+1, lz+1}, {lx+1, ly+1, lz+1});
            float ao3 = calculateAO(world, chunk, chunkPos, {lx+1, ly+1, lz}, {lx, ly+1, lz-1}, {lx+1, ly+1, lz-1});

            mesh.vertices.push_back({{px, py+1, pz}, normal, {uvMin.x, uvMin.y}, textures.Top.Color, ao0, skyLight});
            mesh.vertices.push_back({{px, py+1, pz+1}, normal, {uvMax.x, uvMin.y}, textures.Top.Color, ao1, skyLight});
            mesh.vertices.push_back({{px+1, py+1, pz+1}, normal, {uvMax.x, uvMax.y}, textures.Top.Color, ao2, skyLight});
            mesh.vertices.push_back({{px+1, py+1, pz}, normal, {uvMin.x, uvMax.y}, textures.Top.Color, ao3, skyLight});
            break;
        };
        case FaceDirection::Bottom: {
            glm::vec3 normal{0, -1, 0};

            float ao0 = calculateAO(world, chunk, chunkPos, {lx-1, ly-1, lz}, {lx, ly-1, lz+1}, {lx-1, ly-1, lz+1});
            float ao1 = calculateAO(world, chunk, chunkPos, {lx-1, ly-1, lz}, {lx, ly-1, lz-1}, {lx-1, ly-1, lz-1});
            float ao2 = calculateAO(world, chunk, chunkPos, {lx+1, ly-1, lz}, {lx, ly-1, lz-1}, {lx+1, ly-1, lz-1});
            float ao3 = calculateAO(world, chunk, chunkPos, {lx+1, ly-1, lz}, {lx, ly-1, lz+1}, {lx+1, ly-1, lz+1});

            mesh.vertices.push_back({{px, py, pz+1}, normal, {uvMin.x, uvMin.y}, textures.Bottom.Color, ao0, skyLight});
            mesh.vertices.push_back({{px, py, pz}, normal, {uvMax.x, uvMin.y}, textures.Bottom.Color, ao1, skyLight});
            mesh.vertices.push_back({{px+1, py, pz}, normal, {uvMax.x, uvMax.y}, textures.Bottom.Color, ao2, skyLight});
            mesh.vertices.push_back({{px+1, py, pz+1}, normal, {uvMin.x, uvMax.y}, textures.Bottom.Color, ao3, skyLight});
            break;
        };
        case FaceDirection::Front: {
            glm::vec3 normal{0, 0, 1};

            float ao0 = calculateAO(world, chunk, chunkPos, {lx-1, ly, lz+1}, {lx, ly-1, lz+1}, {lx-1, ly-1, lz+1});
            float ao1 = calculateAO(world, chunk, chunkPos, {lx+1, ly, lz+1}, {lx, ly-1, lz+1}, {lx+1, ly-1, lz+1});
            float ao2 = calculateAO(world, chunk, chunkPos, {lx+1, ly, lz+1}, {lx, ly+1, lz+1}, {lx+1, ly+1, lz+1});
            float ao3 = calculateAO(world, chunk, chunkPos, {lx-1, ly, lz+1}, {lx, ly+1, lz+1}, {lx-1, ly+1, lz+1});

            mesh.vertices.push_back({{px, py, pz+1}, normal, {uvMin.x, uvMin.y}, textures.Side.Color, ao0, skyLight});
            mesh.vertices.push_back({{px+1, py, pz+1}, normal, {uvMax.x, uvMin.y}, textures.Side.Color, ao1, skyLight});
            mesh.vertices.push_back({{px+1, py+1, pz+1}, normal, {uvMax.x, uvMax.y}, textures.Side.Color, ao2, skyLight});
            mesh.vertices.push_back({{px, py+1, pz+1}, normal, {uvMin.x, uvMax.y}, textures.Side.Color, ao3, skyLight});
            break;
        };
        case FaceDirection::Back: {
            glm::vec3 normal{0, 0, -1};

            float ao0 = calculateAO(world, chunk, chunkPos, {lx+1, ly, lz-1}, {lx, ly-1, lz-1}, {lx+1, ly-1, lz-1});
            float ao1 = calculateAO(world, chunk, chunkPos, {lx-1, ly, lz-1}, {lx, ly-1, lz-1}, {lx-1, ly-1, lz-1});
            float ao2 = calculateAO(world, chunk, chunkPos, {lx-1, ly, lz-1}, {lx, ly+1, lz-1}, {lx-1, ly+1, lz-1});
            float ao3 = calculateAO(world, chunk, chunkPos, {lx+1, ly, lz-1}, {lx, ly+1, lz-1}, {lx+1, ly+1, lz-1});

            mesh.vertices.push_back({{px+1, py, pz}, normal, {uvMin.x, uvMin.y}, textures.Side.Color, ao0, skyLight});
            mesh.vertices.push_back({{px, py, pz}, normal, {uvMax.x, uvMin.y}, textures.Side.Color, ao1, skyLight});
            mesh.vertices.push_back({{px, py+1, pz}, normal, {uvMax.x, uvMax.y}, textures.Side.Color, ao2, skyLight});
            mesh.vertices.push_back({{px+1, py+1, pz}, normal, {uvMin.x, uvMax.y}, textures.Side.Color, ao3, skyLight});
            break;
        };
        case FaceDirection::Right: {
            glm::vec3 normal{1, 0, 0};

            float ao0 = calculateAO(world, chunk, chunkPos, {lx+1, ly, lz+1}, {lx+1, ly-1, lz}, {lx+1, ly-1, lz+1});
            float ao1 = calculateAO(world, chunk, chunkPos, {lx+1, ly, lz-1}, {lx+1, ly-1, lz}, {lx+1, ly-1, lz-1});
            float ao2 = calculateAO(world, chunk, chunkPos, {lx+1, ly, lz-1}, {lx+1, ly+1, lz}, {lx+1, ly+1, lz-1});
            float ao3 = calculateAO(world, chunk, chunkPos, {lx+1, ly, lz+1}, {lx+1, ly+1, lz}, {lx+1, ly+1, lz+1});

            mesh.vertices.push_back({{px+1, py, pz+1}, normal, {uvMin.x, uvMin.y}, textures.Side.Color, ao0, skyLight});
            mesh.vertices.push_back({{px+1, py, pz}, normal, {uvMax.x, uvMin.y}, textures.Side.Color, ao1, skyLight});
            mesh.vertices.push_back({{px+1, py+1, pz}, normal, {uvMax.x, uvMax.y}, textures.Side.Color, ao2, skyLight});
            mesh.vertices.push_back({{px+1, py+1, pz+1}, normal, {uvMin.x, uvMax.y}, textures.Side.Color, ao3, skyLight});
            break;
        };
        case FaceDirection::Left: {
            glm::vec3 normal{-1, 0, 0};

            float ao0 = calculateAO(world, chunk, chunkPos, {lx-1, ly, lz-1}, {lx-1, ly-1, lz}, {lx-1, ly-1, lz-1});
            float ao1 = calculateAO(world, chunk, chunkPos, {lx-1, ly, lz+1}, {lx-1, ly-1, lz}, {lx-1, ly-1, lz+1});
            float ao2 = calculateAO(world, chunk, chunkPos, {lx-1, ly, lz+1}, {lx-1, ly+1, lz}, {lx-1, ly+1, lz+1});
            float ao3 = calculateAO(world, chunk, chunkPos, {lx-1, ly, lz-1}, {lx-1, ly+1, lz}, {lx-1, ly+1, lz-1});

            mesh.vertices.push_back({{px, py, pz}, normal, {uvMin.x, uvMin.y}, textures.Side.Color, ao0, skyLight});
            mesh.vertices.push_back({{px, py, pz+1}, normal, {uvMax.x, uvMin.y}, textures.Side.Color, ao1, skyLight});
            mesh.vertices.push_back({{px, py+1, pz+1}, normal, {uvMax.x, uvMax.y}, textures.Side.Color, ao2, skyLight});
            mesh.vertices.push_back({{px, py+1, pz}, normal, {uvMin.x, uvMax.y}, textures.Side.Color, ao3, skyLight});
            break;
        };
    }

    mesh.indices.push_back(baseIndex + 0);
    mesh.indices.push_back(baseIndex + 1);
    mesh.indices.push_back(baseIndex + 2);
    mesh.indices.push_back(baseIndex + 0);
    mesh.indices.push_back(baseIndex + 2);
    mesh.indices.push_back(baseIndex + 3);
}

void Mesher::addCross(MeshData& mesh, World& world, Chunk* chunk, const ChunkPosition& chunkPos, glm::ivec3 localPos, glm::ivec3 worldPos, BlockType type) {
    uint32_t baseIndex = static_cast<uint32_t>(mesh.vertices.size());

    const BlockData& data = BlockRegistry::Get(type);
    const BlockFaceTextures& textures = data.Textures;

    float receivesDiffuse = data.Render == RenderType::Cross ? 0.0f : 1.0f;

    AtlasCoord atlas = textures.Top.Texture;
    glm::vec2 uvMin = atlas.GetUVMin();
    glm::vec2 uvMax = atlas.GetUVMax();

    uint8_t rawLight = chunk->GetSkyLight(localPos.x, localPos.y, localPos.z); // sempre local, é o próprio bloco
    float skyLight = static_cast<float>(rawLight);

    float ao = 3.0f;
    float px = static_cast<float>(worldPos.x), py = static_cast<float>(worldPos.y), pz = static_cast<float>(worldPos.z);

    glm::vec3 normal1{0.0f, 1.0f, 0.0f};

    mesh.vertices.push_back({{px, py, pz}, normal1, {uvMin.x, uvMin.y}, textures.Top.Color, ao, skyLight, receivesDiffuse});
    mesh.vertices.push_back({{px+1, py, pz+1}, normal1, {uvMax.x, uvMin.y}, textures.Top.Color, ao, skyLight, receivesDiffuse});
    mesh.vertices.push_back({{px+1, py+1, pz+1}, normal1, {uvMax.x, uvMax.y}, textures.Top.Color, ao, skyLight, receivesDiffuse});
    mesh.vertices.push_back({{px, py+1, pz}, normal1, {uvMin.x, uvMax.y}, textures.Top.Color, ao, skyLight, receivesDiffuse});

    glm::vec3 normal2{0.0f, 1.0f, 0.0f};

    mesh.vertices.push_back({{px+1, py, pz}, normal2, {uvMin.x, uvMin.y}, textures.Top.Color, ao, skyLight, receivesDiffuse});
    mesh.vertices.push_back({{px, py, pz+1}, normal2, {uvMax.x, uvMin.y}, textures.Top.Color, ao, skyLight, receivesDiffuse});
    mesh.vertices.push_back({{px, py+1, pz+1}, normal2, {uvMax.x, uvMax.y}, textures.Top.Color, ao, skyLight, receivesDiffuse});
    mesh.vertices.push_back({{px+1, py+1, pz}, normal2, {uvMin.x, uvMax.y}, textures.Top.Color, ao, skyLight, receivesDiffuse});

    mesh.indices.push_back(baseIndex + 0);
    mesh.indices.push_back(baseIndex + 1);
    mesh.indices.push_back(baseIndex + 2);
    mesh.indices.push_back(baseIndex + 0);
    mesh.indices.push_back(baseIndex + 2);
    mesh.indices.push_back(baseIndex + 3);

    mesh.indices.push_back(baseIndex + 2);
    mesh.indices.push_back(baseIndex + 1);
    mesh.indices.push_back(baseIndex + 0);
    mesh.indices.push_back(baseIndex + 3);
    mesh.indices.push_back(baseIndex + 2);
    mesh.indices.push_back(baseIndex + 0);

    mesh.indices.push_back(baseIndex + 4);
    mesh.indices.push_back(baseIndex + 5);
    mesh.indices.push_back(baseIndex + 6);
    mesh.indices.push_back(baseIndex + 4);
    mesh.indices.push_back(baseIndex + 6);
    mesh.indices.push_back(baseIndex + 7);

    mesh.indices.push_back(baseIndex + 6);
    mesh.indices.push_back(baseIndex + 5);
    mesh.indices.push_back(baseIndex + 4);
    mesh.indices.push_back(baseIndex + 7);
    mesh.indices.push_back(baseIndex + 6);
    mesh.indices.push_back(baseIndex + 4);
}

FaceData Mesher::getDataForFace(const BlockFaceTextures& textures, FaceDirection direction) {
    switch (direction) {
        case FaceDirection::Top:
            return textures.Top;
        case FaceDirection::Bottom:
            return textures.Bottom;
        default:
            return textures.Side;
    }
}

bool Mesher::shouldRenderFace(BlockType current, BlockType neighbor) {
    const BlockData& currentData = BlockRegistry::Get(current);
    const BlockData& neighborData = BlockRegistry::Get(neighbor);

    if (neighbor == BlockType::Air) return true;
    if (current == neighbor) return false;
    if (currentData.Render == RenderType::Opaque && neighborData.Render == RenderType::Opaque) return false;

    return true;
}

float Mesher::calculateAO(World& world, Chunk* chunk, const ChunkPosition& chunkPos, glm::ivec3 side1, glm::ivec3 side2, glm::ivec3 corner) {
    bool s1 = sampleOccluder(world, chunk, chunkPos, side1.x, side1.y, side1.z);
    bool s2 = sampleOccluder(world, chunk, chunkPos, side2.x, side2.y, side2.z);
    bool c  = sampleOccluder(world, chunk, chunkPos, corner.x, corner.y, corner.z);

    if (s1 && s2) return 0.0f;
    return 3.0f - static_cast<float>(s1) - static_cast<float>(s2) - static_cast<float>(c);
}