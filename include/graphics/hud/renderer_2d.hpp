#pragma once

#include <graphics/atlas_texture.hpp>
#include <graphics/atlas_coord.hpp>
#include <graphics/hud/gui_atlas.hpp>
#include <graphics/shader.hpp>
#include <graphics/hud/mesh_2d.hpp>
#include <graphics/shapes/quad.hpp>

class Renderer2D {
    public:
        Renderer2D() 
            : m_shader("assets/shaders/ui.vert", "assets/shaders/ui.frag"),
              m_quad(CreateUIQuad()) {}

        void Begin(int width, int height);
        void End();

        void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = glm::vec4{1.0f});
        void DrawSprite(const GUIAtlasTexture& atlas, AtlasCoord coord, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = glm::vec4{1.0f});
        void DrawSprite(const AtlasTexture& atlas, AtlasCoord coord, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = glm::vec4{1.0f});
    private:
        Shader m_shader;
        Mesh2D m_quad;

        glm::mat4 m_projection;
};