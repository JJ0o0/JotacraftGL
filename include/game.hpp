#pragma once

#include <platform/window.hpp>
#include <world/world_renderer.hpp>
#include <world/outline_renderer.hpp>
#include <world/block.hpp>
#include <player.hpp>

class Game {
    public:
        Game(Window& window) : m_window(window) {}

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

        BlockType m_placeBlockType = BlockType::Dirt;

        bool m_showDebug = true;
};