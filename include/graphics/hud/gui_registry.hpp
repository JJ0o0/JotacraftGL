#pragma once

#include <world/block.hpp>
#include <graphics/hud/gui_element.hpp>
#include <graphics/atlas_coord.hpp>
#include <array>

class GUIRegistry {
    public:
        static void Init();
        static const AtlasCoord& Get(GUIElement element);
    private:
        static std::array<AtlasCoord, (size_t)GUIElement::Count> s_textures;
};

class BlockIconRegistry {
    public:
        static void Init();
        static const AtlasCoord& Get(BlockType type);
    private:
        static std::array<AtlasCoord, (size_t)BlockType::Count> s_textures;
};