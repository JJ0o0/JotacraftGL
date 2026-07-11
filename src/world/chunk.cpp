#include <world/chunk.hpp>

void Chunk::Update(float deltatime) {

}

void Chunk::GenerateFlat(int groundHeight) {
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                if (y >= groundHeight) {
                    SetVoxel(x, y, z, BlockType::Air);
                    continue;
                }

                BlockType type = BlockType::Grass;

                if (y < (groundHeight - 1) && y >= (groundHeight / 2)) {
                    type = BlockType::Dirt;
                }

                if (y < (groundHeight / 2)) {
                    type = BlockType::Stone;
                }

                SetVoxel(x, y, z, type);
            }
        }
    }
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
    if (x < 0 || x >= CHUNK_SIZE) return false;
    if (y < 0 || y >= CHUNK_SIZE) return false;
    if (z < 0 || z >= CHUNK_SIZE) return false;

    return true;
}

int Chunk::getIndex(int x, int y, int z) const {
    return x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE;
}