#pragma once

#include <graphics/chunk_mesh.hpp>
#include <graphics/light.hpp>
#include <graphics/shader.hpp>
#include <graphics/mesh.hpp>
#include <graphics/atlas_texture.hpp>
#include <player.hpp>
#include <world/world.hpp>
#include <queue>

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
        std::unordered_map<ChunkPosition, ChunkMesh> m_chunkMeshes;
        std::queue<ChunkPosition> m_lightQueue;
        std::queue<ChunkPosition> m_meshQueue;

        Shader m_shader;
        AtlasTexture m_atlas;
        SunLight m_sun{};
};