#pragma once

#include <cstdint>
#include <string>

enum class BlockType : uint8_t {
    Air,
    Stone,
    Cobblestone,
    Bedrock,
    Brick,
    Dirt,
    SimpleGrass,
    GrassBlock,
    Grass,
    OakLog,
    OakPlanks,
    GlassBlock,
    Count
};

inline static std::string BlockTypeToString(BlockType type) {
    switch (type) {
        case BlockType::Air: return "Air";
        case BlockType::Stone: return "Stone";
        case BlockType::Cobblestone: return "Cobblestone";
        case BlockType::Bedrock: return "Bedrock";
        case BlockType::Brick: return "Brick";
        case BlockType::Dirt: return "Dirt";
        case BlockType::SimpleGrass: return "Simple Grass";
        case BlockType::GrassBlock: return "Grass";
        case BlockType::Grass: return "Grass";
        case BlockType::OakLog: return "Oak Log";
        case BlockType::OakPlanks: return "Oak Planks";
        case BlockType::GlassBlock: return "Glass Block";
        default: return "Unknown";
    }
}