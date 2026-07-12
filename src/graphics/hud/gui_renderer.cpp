#include <graphics/hud/gui_renderer.hpp>
#include <graphics/hud/gui_element.hpp>
#include <graphics/hud/gui_registry.hpp>

void GUIRenderer::Render(const AtlasTexture& iconAtlas, int width, int height, int selectedBlock) {
    m_renderer.Begin(width, height);
        auto crosshair = GUIRegistry::Get(GUIElement::Crosshair);

        m_renderer.DrawSprite(
            m_atlas,
            crosshair,
            { width / 2.0f, height / 2.0f },
            { 25, 25}
        );

        m_hotbar.Render(m_renderer, iconAtlas, m_atlas, width, height, selectedBlock);
    m_renderer.End();
}