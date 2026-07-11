#include <world/block_registry.hpp>

std::array<BlockFaceTextures, (size_t)BlockType::Count> BlockRegistry::s_textures{};

void BlockRegistry::Init() {
    s_textures[(size_t)BlockType::Air] = { {0,0}, {0,0}, {0,0} };

    s_textures[(size_t)BlockType::Stone] = { 
        {{0, 0}}, 
        {{0, 0}}, 
        {{0, 0}} 
    };

    s_textures[(size_t)BlockType::Dirt] = { 
        {{1, 0}}, 
        {{1, 0}}, 
        {{1, 0}} 
    };

    s_textures[(size_t)BlockType::Grass] = { 
        {{3, 0}}, 
        {{2, 0}}, 
        {{1, 0}} 
    };

    s_textures[(size_t)BlockType::Oak] = { 
        {{4, 1}}, 
        {{3, 1}}, 
        {{4, 1}} 
    };
}

const BlockFaceTextures& BlockRegistry::Get(BlockType type) {
    return s_textures[(size_t)type];
}