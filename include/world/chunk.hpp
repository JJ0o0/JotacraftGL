#pragma once

#include "block.hpp"
#include <world/noise.hpp>
#include <array>
#include <cstdint>

class Chunk {
    public:
        static constexpr uint32_t CHUNK_SIZE_X = 16;
        static constexpr uint32_t CHUNK_SIZE_Y = 64;
        static constexpr uint32_t CHUNK_SIZE_Z = 16;

        void GenerateFlat(int groundHeight);
        void GenerateTerrain(const Noise& noise);

        uint8_t GetSkyLight(int x, int y, int z) const;
        void SetSkyLight(int x, int y, int z, uint8_t level);

        BlockType GetVoxel(int x, int y, int z) const;
        void SetVoxel(int x, int y, int z, BlockType type);

        bool IsLightInitialized() const { return m_lightInitialized; }
        void MarkLightInitialized() { m_lightInitialized = true; }
    private:
        std::array<BlockType, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z> m_voxels{};
        std::array<uint8_t, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z> m_lightLevels{};

        bool m_lightInitialized = false;

        bool isValid(int x, int y, int z) const;
        int getIndex(int x, int y, int z) const;
};