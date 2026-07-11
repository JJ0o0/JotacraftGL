#include <graphics/mesher.hpp>
#include <world/block_registry.hpp>

MeshData Mesher::GenerateMesh(World& world, const ChunkPosition& chunkPosition) {
    MeshData mesh;
    
    Chunk* chunk = world.GetChunk(chunkPosition);
    if (!chunk) return mesh;
    
    for (int x = 0; x < Chunk::CHUNK_SIZE; x++) {
        for (int y = 0; y < Chunk::CHUNK_SIZE; y++) {
            for (int z = 0; z < Chunk::CHUNK_SIZE; z++) {
                BlockType block = chunk->GetVoxel(x, y, z);
                if (block == BlockType::Air) continue;

                int worldX = chunkPosition.x * Chunk::CHUNK_SIZE + x;
                int worldY = y;
                int worldZ = chunkPosition.z * Chunk::CHUNK_SIZE + z;

                if (world.GetBlock(worldX, worldY + 1, worldZ) == BlockType::Air) {
                    addFace(mesh, {worldX, worldY, worldZ}, FaceDirection::Top, block);
                }

                if (world.GetBlock(worldX, worldY - 1, worldZ) == BlockType::Air) {
                    addFace(mesh, {worldX, worldY, worldZ}, FaceDirection::Bottom, block);
                }

                if (world.GetBlock(worldX, worldY, worldZ + 1) == BlockType::Air) {
                    addFace(mesh, {worldX, worldY, worldZ}, FaceDirection::Front, block);
                }

                if (world.GetBlock(worldX, worldY, worldZ - 1) == BlockType::Air) {
                    addFace(mesh, {worldX, worldY, worldZ}, FaceDirection::Back, block);
                }

                if (world.GetBlock(worldX + 1, worldY, worldZ) == BlockType::Air) {
                    addFace(mesh, {worldX, worldY, worldZ}, FaceDirection::Right, block);
                }

                if (world.GetBlock(worldX - 1, worldY, worldZ) == BlockType::Air) {
                    addFace(mesh, {worldX, worldY, worldZ}, FaceDirection::Left, block);
                }
            }
        }
    }

    return mesh;
}

void Mesher::addFace(MeshData& mesh, glm::vec3 position, FaceDirection direction, BlockType type) {
    uint32_t baseIndex = static_cast<uint32_t>(mesh.vertices.size());
    const BlockFaceTextures& textures = BlockRegistry::Get(type);

    // Vertices
    AtlasCoord atlasCoord = getDataForFace(textures, direction).Texture;
    glm::vec2 uvMin = atlasCoord.GetUVMin();
    glm::vec2 uvMax = atlasCoord.GetUVMax();
    switch (direction) {
        case FaceDirection::Top: {
            glm::vec3 normal{0, 1, 0};

            mesh.vertices.push_back({
                {position.x, position.y + 1, position.z},
                normal,
                {uvMin.x, uvMin.y},
                textures.Top.Color
            });

            mesh.vertices.push_back({
                {position.x, position.y + 1, position.z + 1},
                normal,
                {uvMax.x, uvMin.y},
                textures.Top.Color
            });

            mesh.vertices.push_back({
                {position.x + 1, position.y + 1, position.z + 1},
                normal,
                {uvMax.x, uvMax.y},
                textures.Top.Color
            });

            mesh.vertices.push_back({
                {position.x + 1, position.y + 1, position.z},
                normal,
                {uvMin.x, uvMax.y},
                textures.Top.Color
            });

            break;
        };
        case FaceDirection::Bottom: {
            glm::vec3 normal{0, -1, 0};

            mesh.vertices.push_back({
                {position.x, position.y, position.z + 1},
                normal,
                {uvMin.x, uvMin.y},
                textures.Bottom.Color
            });

            mesh.vertices.push_back({
                {position.x, position.y, position.z},
                normal,
                {uvMax.x, uvMin.y},
                textures.Bottom.Color
            });

            mesh.vertices.push_back({
                {position.x + 1, position.y, position.z},
                normal,
                {uvMax.x, uvMax.y},
                textures.Bottom.Color
            });

            mesh.vertices.push_back({
                {position.x + 1, position.y, position.z + 1},
                normal,
                {uvMin.x, uvMax.y},
                textures.Bottom.Color
            });

            break;
        };
        case FaceDirection::Front: {
            glm::vec3 normal{0, 0, 1};

            mesh.vertices.push_back({
                {position.x, position.y, position.z + 1},
                normal,
                {uvMin.x, uvMin.y},
                textures.Side.Color
            });

            mesh.vertices.push_back({
                {position.x + 1, position.y, position.z + 1},
                normal,
                {uvMax.x, uvMin.y},
                textures.Side.Color
            });

            mesh.vertices.push_back({
                {position.x + 1, position.y + 1, position.z + 1},
                normal,
                {uvMax.x, uvMax.y},
                textures.Side.Color
            });

            mesh.vertices.push_back({
                {position.x, position.y + 1, position.z + 1},
                normal,
                {uvMin.x, uvMax.y},
                textures.Side.Color
            });

            break;
        };
        case FaceDirection::Back: {
            glm::vec3 normal{0, 0, -1};

            mesh.vertices.push_back({
                {position.x + 1, position.y, position.z},
                normal,
                {uvMin.x, uvMin.y},
                textures.Side.Color
            });

            mesh.vertices.push_back({
                {position.x, position.y, position.z},
                normal,
                {uvMax.x, uvMin.y},
                textures.Side.Color
            });

            mesh.vertices.push_back({
                {position.x, position.y + 1, position.z},
                normal,
                {uvMax.x, uvMax.y},
                textures.Side.Color
            });

            mesh.vertices.push_back({
                {position.x + 1, position.y + 1, position.z},
                normal,
                {uvMin.x, uvMax.y},
                textures.Side.Color
            });

            break;
        };
        case FaceDirection::Right: {
            glm::vec3 normal{1, 0, 0};

            mesh.vertices.push_back({
                {position.x + 1, position.y, position.z + 1},
                normal,
                {uvMin.x, uvMin.y},
                textures.Side.Color
            });

            mesh.vertices.push_back({
                {position.x + 1, position.y, position.z},
                normal,
                {uvMax.x, uvMin.y},
                textures.Side.Color
            });

            mesh.vertices.push_back({
                {position.x + 1, position.y + 1, position.z},
                normal,
                {uvMax.x, uvMax.y},
                textures.Side.Color
            });

            mesh.vertices.push_back({
                {position.x + 1, position.y + 1, position.z + 1},
                normal,
                {uvMin.x, uvMax.y},
                textures.Side.Color
            });

            break;
        };
        case FaceDirection::Left: {
            glm::vec3 normal{-1, 0, 0};

            mesh.vertices.push_back({
                {position.x, position.y, position.z},
                normal,
                {uvMin.x, uvMin.y},
                textures.Side.Color
            });

            mesh.vertices.push_back({
                {position.x, position.y, position.z + 1},
                normal,
                {uvMax.x, uvMin.y},
                textures.Side.Color
            });

            mesh.vertices.push_back({
                {position.x, position.y + 1, position.z + 1},
                normal,
                {uvMax.x, uvMax.y},
                textures.Side.Color
            });

            mesh.vertices.push_back({
                {position.x, position.y + 1, position.z},
                normal,
                {uvMin.x, uvMax.y},
                textures.Side.Color
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