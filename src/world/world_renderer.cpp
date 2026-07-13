#include <world/world_renderer.hpp>
#include <graphics/light_engine.hpp>
#include <graphics/mesher.hpp>

void WorldRenderer::Update(World& world, const WorldUpdateResult& update) {
    for (const auto& pos : update.Created) {
        LightEngine::InitializeSkyLight(world, pos);
    }
    
    for (const auto& pos : update.Created) {
        RegenerateChunk(world, pos);

        RegenerateChunk(world, {pos.x - 1, pos.z});
        RegenerateChunk(world, {pos.x + 1, pos.z});
        RegenerateChunk(world, {pos.x, pos.z - 1});
        RegenerateChunk(world, {pos.x, pos.z + 1});
    }

    for (const auto& pos : update.Removed) {
        m_chunkMeshes.erase(pos);
    }
}

void WorldRenderer::Render(const Player& player) {
    glm::mat4 model{1.0f};
    m_shader.Bind();
        m_shader.SetMat4("uModel", model);
        m_shader.SetMat4("uView", player.GetCamera().GetViewMatrix(player.GetEyePosition()));
        m_shader.SetMat4("uProjection", player.GetCamera().GetProjectionMatrix());
        
        m_shader.SetVec3("uSun.Color", m_sun.Color);
        m_shader.SetVec3("uSun.Direction", m_sun.Direction);
        
        m_shader.SetInt("uTexture", 0);

        m_atlas.Bind();
            glDisable(GL_BLEND);
            glDepthMask(GL_TRUE);
                for (auto& [_, chunk] : m_chunkMeshes) chunk.Opaque.Draw();
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthMask(GL_FALSE);
                for (auto& [_, chunk] : m_chunkMeshes) chunk.Transparent.Draw();
            glDepthMask(GL_TRUE);
            glDisable(GL_BLEND);
        m_atlas.Unbind();
    m_shader.Unbind();
}

void WorldRenderer::RegenerateChunk(World& world, const ChunkPosition& pos) {
    if (!world.GetChunk(pos)) return;
    Meshes data = Mesher::GenerateMesh(world, pos);

    auto& chunkMesh = m_chunkMeshes[pos];
    m_chunkMeshes[pos].Opaque = Mesh(data.Opaque.vertices, data.Opaque.indices);
    m_chunkMeshes[pos].Transparent = Mesh(data.Transparent.vertices, data.Transparent.indices);
}