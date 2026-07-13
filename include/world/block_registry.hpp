#pragma once

#include <graphics/atlas_coord.hpp>
#include <world/block.hpp>
#include <array>

enum class RenderType {
    Opaque,
    Cross,
    Transparent
};

struct FaceData {
    AtlasCoord Texture;
    glm::vec3 Color{1.0f};
};

struct BlockFaceTextures {
    FaceData Top;
    FaceData Side;
    FaceData Bottom;
};

struct BlockData {
    BlockFaceTextures Textures;
    RenderType Render = RenderType::Opaque;
    
    bool Solid = true;
    bool BlocksLight = true;
};

class BlockRegistry {
    public:
        static void Init();
        static const BlockData& Get(BlockType type);

    private:
        static std::array<BlockData, (size_t)BlockType::Count> s_blocks;
};