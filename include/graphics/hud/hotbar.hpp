#pragma once

#include <world/block.hpp>
#include <graphics/hud/renderer_2d.hpp>

class Hotbar {
    public:
        void Render(Renderer2D& renderer, const AtlasTexture& iconAtlas, GUIAtlasTexture& atlas, int width, int height, int selected);
    private:
        std::array<BlockType, 9> m_blocks {
            BlockType::Dirt,
            BlockType::Grass,
            BlockType::SimpleGrass,
            BlockType::Stone,
            BlockType::Cobblestone,
            BlockType::Bedrock,
            BlockType::Brick,
            BlockType::OakLog,
            BlockType::OakPlanks
        };
};