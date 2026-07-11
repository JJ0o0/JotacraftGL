#include <world/world_renderer.hpp>
#include <graphics/mesher.hpp>

void WorldRenderer::Update(World& world, const WorldUpdateResult& update) {
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
        m_shader.SetInt("uTexture", 0);

        m_atlas.Bind();
            for (auto& [_, mesh] : m_chunkMeshes) mesh.Draw();
        m_atlas.Unbind();
    m_shader.Unbind();
}

void WorldRenderer::RegenerateChunk(World& world, const ChunkPosition& pos) {
    if (!world.GetChunk(pos)) return;
    MeshData data = Mesher::GenerateMesh(world, pos);
    
    auto it = m_chunkMeshes.find(pos);
    if (it != m_chunkMeshes.end()) it->second = Mesh(data.vertices, data.indices);
    else m_chunkMeshes.emplace(pos, Mesh(data.vertices, data.indices));
}