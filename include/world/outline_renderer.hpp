#pragma once

#include "graphics/shader.hpp"
#include <graphics/shapes/outline_cube.hpp>
#include <player.hpp>

class OutlineRenderer {
    public:
        OutlineRenderer()
            : m_shader("assets/shaders/outline.vert", "assets/shaders/outline.frag"),
              m_mesh(CreateWireframeCube()) {}

        void Update(World& world, Player& player);
        void Render(const Player& player);
    private:
        Shader m_shader;
        Mesh m_mesh;
        RaycastResult m_raycast;
};