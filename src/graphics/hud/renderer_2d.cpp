#include <graphics/hud/renderer_2d.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glad/gl.h>

void Renderer2D::Begin(int width, int height) {
    glDisable(GL_DEPTH_TEST);

    m_shader.Bind();
        m_projection = glm::ortho(
            0.0f,
            (float)width,
            (float)height,
            0.0f
        );

        m_shader.SetMat4("uProjection", m_projection);
}

void Renderer2D::End() {
    m_shader.Unbind();

    glEnable(GL_DEPTH_TEST);
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    glm::mat4 model{1.0f};
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));
    
    m_shader.SetVec2("uUvMin", {0.0f, 0.0f});
    m_shader.SetVec2("uUvMax", {1.0f, 1.0f});
    m_shader.SetMat4("uModel", model);
    m_shader.SetVec4("uTint", color);
    m_shader.SetBool("uHasTexture", false);

    m_quad.Draw();
}

void Renderer2D::DrawSprite(const GUIAtlasTexture& atlas, AtlasCoord coord, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    glm::vec2 uvMin = coord.GetUVMin();
    glm::vec2 uvMax = coord.GetUVMax();
    
    glm::mat4 model{1.0f};
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));
    
    m_shader.SetVec2("uUvMin", uvMin);
    m_shader.SetVec2("uUvMax", uvMax);
    m_shader.SetMat4("uModel", model);
    m_shader.SetVec4("uTint", color);
    m_shader.SetBool("uHasTexture", true);

    atlas.Bind();
        m_quad.Draw();
    atlas.Unbind();
}

void Renderer2D::DrawSprite(const AtlasTexture& atlas, AtlasCoord coord, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    glm::vec2 uvMin = coord.GetUVMin();
    glm::vec2 uvMax = coord.GetUVMax();
    
    glm::mat4 model{1.0f};
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));
    
    m_shader.SetVec2("uUvMin", uvMin);
    m_shader.SetVec2("uUvMax", uvMax);
    m_shader.SetMat4("uModel", model);
    m_shader.SetVec4("uTint", color);
    m_shader.SetBool("uHasTexture", true);

    atlas.Bind();
        m_quad.Draw();
    atlas.Unbind();
}