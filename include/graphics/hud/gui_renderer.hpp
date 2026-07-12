#pragma once

#include <graphics/hud/hotbar.hpp>
#include <graphics/hud/renderer_2d.hpp>
#include <graphics/hud/gui_atlas.hpp>

class GUIRenderer {
    public:
        void Render(const AtlasTexture& iconAtlas, int width, int height, int selectedBlock);
    private:
        Renderer2D m_renderer;
        GUIAtlasTexture m_atlas;

        Hotbar m_hotbar;
};