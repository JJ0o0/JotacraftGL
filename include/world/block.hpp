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
    Grass,
    Oak,
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
        case BlockType::Grass: return "Grass";
        case BlockType::Oak: return "Oak";
        default: return "Unknown";
    }
}