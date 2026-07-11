#pragma once

#include <world/world_renderer.hpp>
#include <world/world.hpp>
#include <player.hpp>

class WorldInteraction {
    public:
        static void PlaceBlock(World& world, Player& player, WorldRenderer& renderer, const RaycastResult& target, BlockType type);
        static void BreakBlock(World& world, WorldRenderer& renderer, const RaycastResult& target);
};