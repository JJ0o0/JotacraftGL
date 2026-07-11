#pragma once

#include <graphics/texture.hpp>

class AtlasTexture {
    public:
        AtlasTexture() {
            m_texture.ChangeImage("assets/textures/atlas.png");
            m_texture.ChangeMinFilter(TextureFilterOption::Nearest);
            m_texture.ChangeMagFilter(TextureFilterOption::Nearest);
            m_texture.ChangeWrap(TextureWrapOption::ClampToEdge);
        }

        void Bind() const { m_texture.Bind(); }
        void Unbind() const { m_texture.Unbind(); }
    private:
        Texture m_texture;
};