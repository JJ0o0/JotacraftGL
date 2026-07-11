#pragma once

#include <graphics/atlas_coord.hpp>
#include <world/block.hpp>
#include <array>

struct FaceData {
    AtlasCoord Texture;
    glm::vec3 Color{1.0f};
};

struct BlockFaceTextures {
    FaceData Top;
    FaceData Side;
    FaceData Bottom;
};

class BlockRegistry {
public:
    static void Init();
    static const BlockFaceTextures& Get(BlockType type);

private:
    static std::array<BlockFaceTextures, (size_t)BlockType::Count> s_textures;
};