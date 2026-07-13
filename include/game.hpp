#pragma once

#include <platform/window.hpp>
#include <world/world_renderer.hpp>
#include <world/outline_renderer.hpp>
#include <graphics/hud/gui_renderer.hpp>
#include <world/block.hpp>
#include <player.hpp>

class Game {
    public:
        Game(Window& window);

        void Initialize();
        void Update(float deltatime);
        void Render();
        void RenderDebugGUI();
    private:
        Window& m_window;
        Player m_player{};
        World m_world{};
        WorldRenderer m_worldRenderer{};
        OutlineRenderer m_outlineRenderer{};
        GUIRenderer m_guiRenderer{};

        std::array<BlockType, 9> m_hotbarBlocks {
            BlockType::Dirt,
            BlockType::GrassBlock,
            BlockType::SimpleGrass,
            BlockType::Stone,
            BlockType::Cobblestone,
            BlockType::Brick,
            BlockType::OakLog,
            BlockType::OakPlanks,
            BlockType::GlassBlock
        };

        int m_selectedSlot = 0;

        BlockType GetSelectedBlock() const { return m_hotbarBlocks[m_selectedSlot]; }

        bool m_showDebug = true;
};