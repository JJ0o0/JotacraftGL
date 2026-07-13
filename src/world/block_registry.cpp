#include <world/block_registry.hpp>

std::array<BlockData, (size_t)BlockType::Count> BlockRegistry::s_blocks{};

void BlockRegistry::Init() {
    s_blocks[(size_t)BlockType::Air] = {
        {
            {{0,0}},
            {{0,0}},
            {{0,0}}
        },
        RenderType::Transparent,
        false,
        false,
    };

    s_blocks[(size_t)BlockType::Stone] = {
        {
            {{0, 0}},
            {{0, 0}},
            {{0, 0}}
        },
    };

    s_blocks[(size_t)BlockType::Cobblestone] = {
        {
            {{0, 1}},
            {{0, 1}},
            {{0, 1}}
        },
    };

    s_blocks[(size_t)BlockType::Bedrock] = {
        {
            {{1, 1}},
            {{1, 1}},
            {{1, 1}}
        },
    };

    s_blocks[(size_t)BlockType::Brick] = {
        {
            {{5, 0}},
            {{5, 0}},
            {{5, 0}}
        },
    };

    s_blocks[(size_t)BlockType::Dirt] = {
        {
            {{1, 0}},
            {{1, 0}},
            {{1, 0}}
        },
    };

    s_blocks[(size_t)BlockType::GrassBlock] = {
        {
            {{3, 0}},
            {{2, 0}},
            {{1, 0}}
        },
    };

    s_blocks[(size_t)BlockType::Grass] = {
        {
            {{1, 2}},
            {{1, 2}},
            {{1, 2}}
        },
        RenderType::Cross,
        false,
        false,
    };

    s_blocks[(size_t)BlockType::SimpleGrass] = {
        {
            {{3, 0}},
            {{3, 0}},
            {{3, 0}}
        },
    };

    s_blocks[(size_t)BlockType::OakLog] = {
        {
            {{4, 1}},
            {{3, 1}},
            {{4, 1}}
        },
    };

    s_blocks[(size_t)BlockType::OakPlanks] = {
        {
            {{4, 0}},
            {{4, 0}},
            {{4, 0}}
        },
    };

    s_blocks[(size_t)BlockType::GlassBlock] = {
        {
            {{0, 2}},
            {{0, 2}},
            {{0, 2}}
        },
        RenderType::Transparent,
        true,
        false
    };
}

const BlockData& BlockRegistry::Get(BlockType type) {
    return s_blocks[(size_t)type];
}