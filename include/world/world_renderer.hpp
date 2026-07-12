#pragma once

#include <graphics/shader.hpp>
#include <graphics/mesh.hpp>
#include <graphics/atlas_texture.hpp>
#include <player.hpp>
#include <world/world.hpp>

class WorldRenderer {
    public:
        WorldRenderer() 
            : m_shader("assets/shaders/default.vert", "assets/shaders/default.frag"),
              m_atlas() {}

        void Update(World& world, const WorldUpdateResult& update);
        void Render(const Player& player);

        void RegenerateChunk(World& world, const ChunkPosition& pos);

        const AtlasTexture& GetAtlas() const { return m_atlas; }
    private:
        std::unordered_map<ChunkPosition, Mesh> m_chunkMeshes;

        Shader m_shader;
        AtlasTexture m_atlas;
};