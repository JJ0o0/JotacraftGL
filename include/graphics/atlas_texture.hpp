#pragma once

#include <graphics/texture.hpp>
#include <memory>

class AtlasTexture {
    public:
        AtlasTexture() {
            m_texture = std::make_unique<Texture>(TextureProperties{
                .ImagePath = "assets/textures/atlas.png",
                .Wrap = TextureWrapOption::ClampToEdge,
                .MinFilter = TextureFilterOption::Nearest,
                .MagFilter = TextureFilterOption::Nearest,
            });
        }

        void Bind() const { m_texture->Bind(); }
        void Unbind() const { m_texture->Unbind(); }
    private:
        std::unique_ptr<Texture> m_texture;
};