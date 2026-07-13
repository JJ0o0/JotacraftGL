#include <world/outline_renderer.hpp>
#include <glm/ext/matrix_transform.hpp>

void OutlineRenderer::Update(World& world, Player& player) {
    m_raycast = player.Raycast(world, 6.0f);
}

void OutlineRenderer::Render(const Player& player) {
    if (!m_raycast.Hit) return;
    
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_raycast.VoxelPosition));
    model = glm::translate(model, glm::vec3(0.5f));
    model = glm::scale(model, glm::vec3(1.002f));
    model = glm::translate(model, glm::vec3(-0.5f));

    m_shader.Bind();
        m_shader.SetMat4("uModel", model);
        m_shader.SetMat4("uView", player.GetCamera().GetViewMatrix(player.GetEyePosition()));
        m_shader.SetMat4("uProjection", player.GetCamera().GetProjectionMatrix());
        m_shader.SetVec3("uColor", glm::vec3(0.0f));
        m_shader.SetFloat("uThickness", 0.008f);
        
        glDisable(GL_CULL_FACE);
        glDisable(GL_CULL_FACE);
        glDepthMask(GL_FALSE);
            m_mesh.Draw();
        glDepthMask(GL_TRUE);
        glEnable(GL_CULL_FACE);
        glEnable(GL_CULL_FACE);
    m_shader.Unbind();
}