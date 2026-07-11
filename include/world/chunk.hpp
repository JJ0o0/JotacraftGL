#pragma once

#include "block.hpp"
#include <array>
#include <cstdint>

class Chunk {
    public:
        static constexpr uint32_t CHUNK_SIZE = 16;

        void Update(float deltatime);
        void GenerateFlat(int groundHeight);

        BlockType GetVoxel(int x, int y, int z) const;
        void SetVoxel(int x, int y, int z, BlockType type);
    private:
        std::array<BlockType, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> m_voxels;

        bool isValid(int x, int y, int z) const;
        int getIndex(int x, int y, int z) const;
};