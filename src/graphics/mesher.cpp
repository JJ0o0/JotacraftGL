#include <graphics/mesher.hpp>
#include <world/block_registry.hpp>

Meshes Mesher::GenerateMesh(World& world, const ChunkPosition& chunkPosition) {
    Meshes meshes;
    
    Chunk* chunk = world.GetChunk(chunkPosition);
    if (!chunk) return meshes;
    
    for (int x = 0; x < Chunk::CHUNK_SIZE; x++) {
        for (int y = 0; y < Chunk::CHUNK_SIZE; y++) {
            for (int z = 0; z < Chunk::CHUNK_SIZE; z++) {
                BlockType block = chunk->GetVoxel(x, y, z);
                if (block == BlockType::Air) continue;
                
                int worldX = chunkPosition.x * Chunk::CHUNK_SIZE + x;
                int worldY = y;
                int worldZ = chunkPosition.z * Chunk::CHUNK_SIZE + z;
                
                const BlockData& data = BlockRegistry::Get(block);
                MeshData* mesh;

                switch (data.Render) {
                    case RenderType::Opaque:
                        mesh = &meshes.Opaque;
                        break;
                    case RenderType::Transparent:
                        mesh = &meshes.Transparent;
                        break;
                    case RenderType::Cross:
                        addCross(*mesh, world, { worldX, worldY, worldZ }, block);
                        continue;
                }

                BlockType neighbor = world.GetBlock(worldX, worldY + 1, worldZ);
                if (shouldRenderFace(block, neighbor)) {
                    addFace(*mesh, world, {worldX, worldY, worldZ}, {worldX, worldY + 1, worldZ}, FaceDirection::Top, block);
                }

                neighbor = world.GetBlock(worldX, worldY - 1, worldZ);
                if (shouldRenderFace(block, neighbor)) {
                    addFace(*mesh, world, {worldX, worldY, worldZ}, {worldX, worldY - 1, worldZ}, FaceDirection::Bottom, block);
                }

                neighbor = world.GetBlock(worldX, worldY, worldZ + 1);
                if (shouldRenderFace(block, neighbor)) {
                    addFace(*mesh, world, {worldX, worldY, worldZ},{worldX, worldY, worldZ + 1}, FaceDirection::Front, block);
                }

                neighbor = world.GetBlock(worldX, worldY, worldZ - 1);
                if (shouldRenderFace(block, neighbor)) {
                    addFace(*mesh, world, {worldX, worldY, worldZ}, {worldX, worldY, worldZ - 1}, FaceDirection::Back, block);
                }

                neighbor = world.GetBlock(worldX + 1, worldY, worldZ);
                if (shouldRenderFace(block, neighbor)) {
                    addFace(*mesh, world, {worldX, worldY, worldZ}, {worldX + 1, worldY, worldZ}, FaceDirection::Right, block);
                }

                neighbor = world.GetBlock(worldX - 1, worldY, worldZ);
                if (shouldRenderFace(block, neighbor)) {
                    addFace(*mesh, world, {worldX, worldY, worldZ}, {worldX - 1, worldY, worldZ}, FaceDirection::Left, block);
                }
            }
        }
    }

    return meshes;
}

void Mesher::addFace(MeshData& mesh, World& world, glm::vec3 position, glm::ivec3 neighborPosition, FaceDirection direction, BlockType type) {
    uint32_t baseIndex = static_cast<uint32_t>(mesh.vertices.size());
    const BlockData& data = BlockRegistry::Get(type);
    const BlockFaceTextures& textures = data.Textures;

    uint8_t rawLight = world.GetSkyLight(neighborPosition.x, neighborPosition.y, neighborPosition.z);
    float skyLight = static_cast<float>(rawLight);

    // Vertices
    AtlasCoord atlasCoord = getDataForFace(textures, direction).Texture;
    glm::vec2 uvMin = atlasCoord.GetUVMin();
    glm::vec2 uvMax = atlasCoord.GetUVMax();

    int bx = static_cast<int>(position.x);
    int by = static_cast<int>(position.y);
    int bz = static_cast<int>(position.z);
    switch (direction) {
        case FaceDirection::Top: {
            glm::vec3 normal{0, 1, 0};

            float ao0 = calculateAO(world, {bx-1, by+1, bz}, {bx, by+1, bz-1}, {bx-1, by+1, bz-1});
            float ao1 = calculateAO(world, {bx-1, by+1, bz}, {bx, by+1, bz+1}, {bx-1, by+1, bz+1});
            float ao2 = calculateAO(world, {bx+1, by+1, bz}, {bx, by+1, bz+1}, {bx+1, by+1, bz+1});
            float ao3 = calculateAO(world, {bx+1, by+1, bz}, {bx, by+1, bz-1}, {bx+1, by+1, bz-1});

            mesh.vertices.push_back({
                {position.x, position.y + 1, position.z},
                normal,
                {uvMin.x, uvMin.y},
                textures.Top.Color,
                ao0,
                skyLight
            });

            mesh.vertices.push_back({
                {position.x, position.y + 1, position.z + 1},
                normal,
                {uvMax.x, uvMin.y},
                textures.Top.Color,
                ao1,
                skyLight
            });

            mesh.vertices.push_back({
                {position.x + 1, position.y + 1, position.z + 1},
                normal,
                {uvMax.x, uvMax.y},
                textures.Top.Color,
                ao2,
                skyLight
            });

            mesh.vertices.push_back({
                {position.x + 1, position.y + 1, position.z},
                normal,
                {uvMin.x, uvMax.y},
                textures.Top.Color,
                ao3,
                skyLight
            });

            break;
        };
        case FaceDirection::Bottom: {
            glm::vec3 normal{0, -1, 0};

            float ao0 = calculateAO(world, {bx-1, by-1, bz}, {bx, by-1, bz+1}, {bx-1, by-1, bz+1});
            float ao1 = calculateAO(world, {bx-1, by-1, bz}, {bx, by-1, bz-1}, {bx-1, by-1, bz-1});
            float ao2 = calculateAO(world, {bx+1, by-1, bz}, {bx, by-1, bz-1}, {bx+1, by-1, bz-1});
            float ao3 = calculateAO(world, {bx+1, by-1, bz}, {bx, by-1, bz+1}, {bx+1, by-1, bz+1});

            mesh.vertices.push_back({
                {position.x, position.y, position.z + 1},
                normal,
                {uvMin.x, uvMin.y},
                textures.Bottom.Color,
                ao0,
                skyLight
            });

            mesh.vertices.push_back({
                {position.x, position.y, position.z},
                normal,
                {uvMax.x, uvMin.y},
                textures.Bottom.Color,
                ao1,
                skyLight
            });

            mesh.vertices.push_back({
                {position.x + 1, position.y, position.z},
                normal,
                {uvMax.x, uvMax.y},
                textures.Bottom.Color,
                ao2,
                skyLight
            });

            mesh.vertices.push_back({
                {position.x + 1, position.y, position.z + 1},
                normal,
                {uvMin.x, uvMax.y},
                textures.Bottom.Color,
                ao3,
                skyLight
            });

            break;
        };
        case FaceDirection::Front: {
            glm::vec3 normal{0, 0, 1};

            float ao0 = calculateAO(world, {bx-1, by, bz+1}, {bx, by-1, bz+1}, {bx-1, by-1, bz+1});
            float ao1 = calculateAO(world, {bx+1, by, bz+1}, {bx, by-1, bz+1}, {bx+1, by-1, bz+1});
            float ao2 = calculateAO(world, {bx+1, by, bz+1}, {bx, by+1, bz+1}, {bx+1, by+1, bz+1});
            float ao3 = calculateAO(world, {bx-1, by, bz+1}, {bx, by+1, bz+1}, {bx-1, by+1, bz+1});

            mesh.vertices.push_back({
                {position.x, position.y, position.z + 1},
                normal,
                {uvMin.x, uvMin.y},
                textures.Side.Color,
                ao0,
                skyLight
            });

            mesh.vertices.push_back({
                {position.x + 1, position.y, position.z + 1},
                normal,
                {uvMax.x, uvMin.y},
                textures.Side.Color,
                ao1,
                skyLight
            });

            mesh.vertices.push_back({
                {position.x + 1, position.y + 1, position.z + 1},
                normal,
                {uvMax.x, uvMax.y},
                textures.Side.Color,
                ao2,
                skyLight
            });

            mesh.vertices.push_back({
                {position.x, position.y + 1, position.z + 1},
                normal,
                {uvMin.x, uvMax.y},
                textures.Side.Color,
                ao3,
                skyLight
            });

            break;
        };
        case FaceDirection::Back: {
            glm::vec3 normal{0, 0, -1};

            float ao0 = calculateAO(world, {bx+1, by, bz-1}, {bx, by-1, bz-1}, {bx+1, by-1, bz-1});
            float ao1 = calculateAO(world, {bx-1, by, bz-1}, {bx, by-1, bz-1}, {bx-1, by-1, bz-1});
            float ao2 = calculateAO(world, {bx-1, by, bz-1}, {bx, by+1, bz-1}, {bx-1, by+1, bz-1});
            float ao3 = calculateAO(world, {bx+1, by, bz-1}, {bx, by+1, bz-1}, {bx+1, by+1, bz-1});

            mesh.vertices.push_back({
                {position.x + 1, position.y, position.z},
                normal,
                {uvMin.x, uvMin.y},
                textures.Side.Color,
                ao0,
                skyLight
            });

            mesh.vertices.push_back({
                {position.x, position.y, position.z},
                normal,
                {uvMax.x, uvMin.y},
                textures.Side.Color,
                ao1,
                skyLight
            });

            mesh.vertices.push_back({
                {position.x, position.y + 1, position.z},
                normal,
                {uvMax.x, uvMax.y},
                textures.Side.Color,
                ao2,
                skyLight
            });

            mesh.vertices.push_back({
                {position.x + 1, position.y + 1, position.z},
                normal,
                {uvMin.x, uvMax.y},
                textures.Side.Color,
                ao3,
                skyLight
            });

            break;
        };
        case FaceDirection::Right: {
            glm::vec3 normal{1, 0, 0};

            float ao0 = calculateAO(world, {bx+1, by, bz+1}, {bx+1, by-1, bz}, {bx+1, by-1, bz+1});
            float ao1 = calculateAO(world, {bx+1, by, bz-1}, {bx+1, by-1, bz}, {bx+1, by-1, bz-1});
            float ao2 = calculateAO(world, {bx+1, by, bz-1}, {bx+1, by+1, bz}, {bx+1, by+1, bz-1});
            float ao3 = calculateAO(world, {bx+1, by, bz+1}, {bx+1, by+1, bz}, {bx+1, by+1, bz+1});

            mesh.vertices.push_back({
                {position.x + 1, position.y, position.z + 1},
                normal,
                {uvMin.x, uvMin.y},
                textures.Side.Color,
                ao0,
                skyLight
            });

            mesh.vertices.push_back({
                {position.x + 1, position.y, position.z},
                normal,
                {uvMax.x, uvMin.y},
                textures.Side.Color,
                ao1,
                skyLight
            });

            mesh.vertices.push_back({
                {position.x + 1, position.y + 1, position.z},
                normal,
                {uvMax.x, uvMax.y},
                textures.Side.Color,
                ao2,
                skyLight
            });

            mesh.vertices.push_back({
                {position.x + 1, position.y + 1, position.z + 1},
                normal,
                {uvMin.x, uvMax.y},
                textures.Side.Color,
                ao3,
                skyLight
            });

            break;
        };
        case FaceDirection::Left: {
            glm::vec3 normal{-1, 0, 0};

            float ao0 = calculateAO(world, {bx-1, by, bz-1}, {bx-1, by-1, bz}, {bx-1, by-1, bz-1});
            float ao1 = calculateAO(world, {bx-1, by, bz+1}, {bx-1, by-1, bz}, {bx-1, by-1, bz+1});
            float ao2 = calculateAO(world, {bx-1, by, bz+1}, {bx-1, by+1, bz}, {bx-1, by+1, bz+1});
            float ao3 = calculateAO(world, {bx-1, by, bz-1}, {bx-1, by+1, bz}, {bx-1, by+1, bz-1});

            mesh.vertices.push_back({
                {position.x, position.y, position.z},
                normal,
                {uvMin.x, uvMin.y},
                textures.Side.Color,
                ao0,
                skyLight
            });

            mesh.vertices.push_back({
                {position.x, position.y, position.z + 1},
                normal,
                {uvMax.x, uvMin.y},
                textures.Side.Color,
                ao1,
                skyLight
            });

            mesh.vertices.push_back({
                {position.x, position.y + 1, position.z + 1},
                normal,
                {uvMax.x, uvMax.y},
                textures.Side.Color,
                ao2,
                skyLight
            });

            mesh.vertices.push_back({
                {position.x, position.y + 1, position.z},
                normal,
                {uvMin.x, uvMax.y},
                textures.Side.Color,
                ao3,
                skyLight
            });

            break;
        };
    }

    // Indices
    mesh.indices.push_back(baseIndex + 0);
    mesh.indices.push_back(baseIndex + 1);
    mesh.indices.push_back(baseIndex + 2);
    mesh.indices.push_back(baseIndex + 0);
    mesh.indices.push_back(baseIndex + 2);
    mesh.indices.push_back(baseIndex + 3);
}

void Mesher::addCross(MeshData& mesh, World& world, glm::vec3 position, BlockType type) {
    uint32_t baseIndex = static_cast<uint32_t>(mesh.vertices.size());

    const BlockData& data = BlockRegistry::Get(type);
    const BlockFaceTextures& textures = data.Textures;

    float receivesDiffuse = data.Render == RenderType::Cross ? 0.0f : 1.0f;

    AtlasCoord atlas = textures.Top.Texture;

    glm::vec2 uvMin = atlas.GetUVMin();
    glm::vec2 uvMax = atlas.GetUVMax();

    uint8_t rawLight = world.GetSkyLight(position.x, position.y, position.z);
    float skyLight = static_cast<float>(rawLight);

    float ao = 3.0f;

    // Blade 01
    glm::vec3 normal1 = glm::vec3(0.0f, 1.0f, 0.0f);

    mesh.vertices.push_back({
        {position.x, position.y, position.z},
        normal1,
        {uvMin.x, uvMin.y},
        textures.Top.Color,
        ao,
        skyLight,
        receivesDiffuse
    });

    mesh.vertices.push_back({
        {position.x + 1, position.y, position.z + 1},
        normal1,
        {uvMax.x, uvMin.y},
        textures.Top.Color,
        ao,
        skyLight,
        receivesDiffuse
    });

    mesh.vertices.push_back({
        {position.x + 1, position.y + 1, position.z + 1},
        normal1,
        {uvMax.x, uvMax.y},
        textures.Top.Color,
        ao,
        skyLight,
        receivesDiffuse
    });

    mesh.vertices.push_back({
        {position.x, position.y + 1, position.z},
        normal1,
        {uvMin.x, uvMax.y},
        textures.Top.Color,
        ao,
        skyLight,
        receivesDiffuse
    });

    // Blade 02
    glm::vec3 normal2 = glm::vec3(0.0f, 1.0f, 0.0f);

    mesh.vertices.push_back({
        {position.x + 1, position.y, position.z},
        normal2,
        {uvMin.x, uvMin.y},
        textures.Top.Color,
        ao,
        skyLight,
        receivesDiffuse
    });

    mesh.vertices.push_back({
        {position.x, position.y, position.z + 1},
        normal2,
        {uvMax.x, uvMin.y},
        textures.Top.Color,
        ao,
        skyLight,
        receivesDiffuse
    });

    mesh.vertices.push_back({
        {position.x, position.y + 1, position.z + 1},
        normal2,
        {uvMax.x, uvMax.y},
        textures.Top.Color,
        ao,
        skyLight,
        receivesDiffuse
    });

    mesh.vertices.push_back({
        {position.x + 1, position.y + 1, position.z},
        normal2,
        {uvMin.x, uvMax.y},
        textures.Top.Color,
        ao,
        skyLight,
        receivesDiffuse
    });

    // Blade 01
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

    // Blade 02
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

bool Mesher::isOccluder(World& world, glm::ivec3 pos) {
    BlockType block = world.GetBlock(pos.x, pos.y, pos.z);

    if (block == BlockType::Air) return false;
    return BlockRegistry::Get(block).BlocksLight;
}

bool Mesher::shouldRenderFace(BlockType current, BlockType neighbor) {
    const BlockData& currentData = BlockRegistry::Get(current);
    const BlockData& neighborData = BlockRegistry::Get(neighbor);

    if (neighbor == BlockType::Air) return true;
    if (current == neighbor) return false;
    if (currentData.Render == RenderType::Opaque && neighborData.Render == RenderType::Opaque) return false;

    return true;
}

float Mesher::calculateAO(World& world, glm::ivec3 side1, glm::ivec3 side2, glm::ivec3 corner) {
    bool s1 = isOccluder(world, side1);
    bool s2 = isOccluder(world, side2);
    bool c = isOccluder(world, corner);

    if (s1 && s2) return 0.0f;
    return 3.0f - static_cast<float>(s1) - static_cast<float>(s2) - static_cast<float>(c);
}