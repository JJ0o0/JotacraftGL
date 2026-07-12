#pragma once

#include "block.hpp"
#include <array>
#include <cstdint>

class Chunk {
    public:
        static constexpr uint32_t CHUNK_SIZE = 16;

        void Update(float deltatime);
        void GenerateFlat(int groundHeight);

        uint8_t GetSkyLight(int x, int y, int z) const;
        void SetSkyLight(int x, int y, int z, uint8_t level);

        BlockType GetVoxel(int x, int y, int z) const;
        void SetVoxel(int x, int y, int z, BlockType type);
    private:
        std::array<BlockType, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> m_voxels{};
        std::array<uint8_t, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> m_lightLevels{};

        bool isValid(int x, int y, int z) const;
        int getIndex(int x, int y, int z) const;
};