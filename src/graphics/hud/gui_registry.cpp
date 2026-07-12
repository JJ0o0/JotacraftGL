#include <graphics/hud/gui_registry.hpp>

std::array<AtlasCoord, (size_t)GUIElement::Count> GUIRegistry::s_textures{};
std::array<AtlasCoord, (size_t)BlockType::Count> BlockIconRegistry::s_textures{};

void GUIRegistry::Init() {
    s_textures[(size_t)GUIElement::Crosshair] = {0, 0};
    s_textures[(size_t)GUIElement::Slot] = {1, 0};
    s_textures[(size_t)GUIElement::SelectedSlot] = {2, 0};
}

const AtlasCoord& GUIRegistry::Get(GUIElement element) {
    return s_textures[(size_t)element];
}

void BlockIconRegistry::Init() {
    s_textures[(size_t)BlockType::Stone] = {0, 0};
    s_textures[(size_t)BlockType::Cobblestone] = {0, 1};
    s_textures[(size_t)BlockType::Bedrock] = {1, 1};
    s_textures[(size_t)BlockType::Brick] = {5, 0};
    s_textures[(size_t)BlockType::Dirt] = { 1, 0 };
    s_textures[(size_t)BlockType::Grass] = { 2, 0};
    s_textures[(size_t)BlockType::SimpleGrass] = {3, 0 };
    s_textures[(size_t)BlockType::OakLog] = {3, 1};
    s_textures[(size_t)BlockType::OakPlanks] = {4, 0};
}

const AtlasCoord& BlockIconRegistry::Get(BlockType type) {
    return s_textures[(size_t)type];
}