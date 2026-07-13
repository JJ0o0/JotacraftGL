#include <world/chunk.hpp>

void Chunk::GenerateFlat(int groundHeight) {
    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int z = 0; z < CHUNK_SIZE_Z; z++) {
                if (y >= groundHeight) {
                    SetVoxel(x, y, z, BlockType::Air);
                    continue;
                }

                BlockType type = BlockType::GrassBlock;

                if (y < (groundHeight - 1) && y >= (groundHeight / 2)) {
                    type = BlockType::Dirt;
                }

                if (y < (groundHeight / 2)) {
                    type = BlockType::Stone;
                }

                if (y == 0) type = BlockType::Bedrock;

                SetVoxel(x, y, z, type);
            }
        }
    }
}

void Chunk::GenerateTerrain(const Noise& noise, int chunkX, int chunkZ) {
    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int z = 0; z < CHUNK_SIZE_Z; z++) {
            int worldX = chunkX * CHUNK_SIZE_X + x;
            int worldZ = chunkZ * CHUNK_SIZE_Z + z;

            float value = noise.Sample(worldX, worldZ);
            int height = 32 + value * 26;

            for (int y = 0; y < CHUNK_SIZE_Y; y++) {
                if (y >= height) {
                    SetVoxel(x, y, z, BlockType::Air);
                    continue;
                }

                BlockType type = BlockType::Stone;
                if (y == height - 1) type = BlockType::GrassBlock;
                else if (y >= height - 4) type = BlockType::Dirt;

                if (y == 0) type = BlockType::Bedrock;

                SetVoxel(x, y, z, type);
            }
        }
    }
}

uint8_t Chunk::GetSkyLight(int x, int y, int z) const {
    if (!isValid(x, y, z)) return 0;

    return m_lightLevels[getIndex(x, y, z)];
}

void Chunk::SetSkyLight(int x, int y, int z, uint8_t level) {
    if (!isValid(x, y, z)) return;

    m_lightLevels[getIndex(x, y, z)] = level;
}

BlockType Chunk::GetVoxel(int x, int y, int z) const {
    if (!isValid(x, y, z)) return BlockType::Air;

    return m_voxels.at(getIndex(x, y, z));
}

void Chunk::SetVoxel(int x, int y, int z, BlockType type) {
    if (!isValid(x, y, z)) return;

    m_voxels.at(getIndex(x, y, z)) = type;
}

bool Chunk::isValid(int x, int y, int z) const {
    if (x < 0 || x >= CHUNK_SIZE_X) return false;
    if (y < 0 || y >= CHUNK_SIZE_Y) return false;
    if (z < 0 || z >= CHUNK_SIZE_Z) return false;

    return true;
}

int Chunk::getIndex(int x, int y, int z) const {
    return x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y;
}