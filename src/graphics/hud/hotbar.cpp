#include <graphics/hud/hotbar.hpp>
#include <graphics/hud/gui_registry.hpp>

void Hotbar::Render(Renderer2D& renderer, const AtlasTexture& iconAtlas, GUIAtlasTexture& atlas, int width, int height, int selected) {
    float slotSize = 45.0f;
    float spacing = 1.0f;

    int slotCount = m_blocks.size();

    float hotbarWidth = slotCount * slotSize + (slotCount - 1) * spacing;

    float startX = (width - hotbarWidth) / 2.0f + slotSize / 2.0f;
    float y = height - (slotSize / 2.0f) - 10.0f;

    auto slotTexture = GUIRegistry::Get(GUIElement::Slot);
    auto selectedSlotTexture = GUIRegistry::Get(GUIElement::SelectedSlot);

    for (int i = 0; i < m_blocks.size(); i++) {
        glm::vec2 pos {
            startX + i * (slotSize + spacing),
            y
        };

        bool selectedSlot = i == selected;
        renderer.DrawSprite(
            atlas,
            selectedSlot ? selectedSlotTexture : slotTexture,
            pos,
            {slotSize, slotSize}
        );

        auto icon = BlockIconRegistry::Get(m_blocks[i]);
        glm::vec2 normalSize = {slotSize / 1.7f,slotSize / 1.7f};
        glm::vec2 selectedSize = {slotSize / 1.45f,slotSize / 1.45f};
        renderer.DrawSprite(
            iconAtlas,
            icon,
            pos,
            selectedSlot ? selectedSize : normalSize
        );
    }
}