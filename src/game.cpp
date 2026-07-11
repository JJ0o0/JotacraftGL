#include <game.hpp>

#include <world/block_registry.hpp>
#include <world/world_interaction.hpp>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

void Game::Initialize() {
    BlockRegistry::Init();

    m_window.WasKeyPressed = [&](int key) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                if (!m_window.IsMouseLocked()) m_player.GetCamera().ResetMouseMovement();

                m_window.ToggleMouseLock();

                break;

            case GLFW_KEY_1: m_placeBlockType = BlockType::Dirt; break;
            case GLFW_KEY_2: m_placeBlockType = BlockType::Grass; break;
            case GLFW_KEY_3: m_placeBlockType = BlockType::SimpleGrass; break;
            case GLFW_KEY_4: m_placeBlockType = BlockType::Stone; break;
            case GLFW_KEY_5: m_placeBlockType = BlockType::Cobblestone; break;
            case GLFW_KEY_6: m_placeBlockType = BlockType::Bedrock; break;
            case GLFW_KEY_7: m_placeBlockType = BlockType::Brick; break;
            case GLFW_KEY_8: m_placeBlockType = BlockType::Oak; break;

            case GLFW_KEY_F3: m_showDebug = !m_showDebug; break;
        }
    };

    m_window.WasMouseButtonPressed = [&](int button) {
        RaycastResult target = m_player.Raycast(m_world, 6.0f);

        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                WorldInteraction::BreakBlock(m_world, m_worldRenderer, target);
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                WorldInteraction::PlaceBlock(m_world, m_player, m_worldRenderer, target, m_placeBlockType);
                break;
        }
    };

    m_window.OnMouseMove = [&](double xpos, double ypos) {
        if (m_window.IsMouseLocked()) m_player.GetCamera().HandleMouseMovement(xpos, ypos);
    };

    auto initResult = m_world.Update(m_player.GetPosition());
    m_worldRenderer.Update(m_world, initResult);
}

void Game::Update(float deltatime) {
    m_player.HandleMovement(m_window.GetHandle(), deltatime);
    m_player.Update(m_world, deltatime);

    WorldUpdateResult result = m_world.Update(m_player.GetPosition());
    m_worldRenderer.Update(m_world, result);
    m_outlineRenderer.Update(m_world, m_player);
}

void Game::Render() {
    m_worldRenderer.Render(m_player);
    m_outlineRenderer.Render(m_player);
}

void Game::RenderDebugGUI() {
    if (!m_showDebug) return;
    
    ImGui::SetNextWindowPos({15, 15});
    ImGui::SetNextWindowSize({300, 100});
    ImGui::Begin("Debug", nullptr,
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize
    );

    auto pos = m_player.GetPosition();
    auto vel = m_player.GetVelocity();
    std::string block = BlockTypeToString(m_placeBlockType);

    ImGui::Text("F3 to open or close");
    ImGui::Text("Player Position: x %.2f, y %.2f, z %.2f", pos.x, pos.y, pos.z);
    ImGui::Text("Player Velocity: x %.2f, y %.2f, z %.2f", vel.x, vel.y, vel.z);
    ImGui::Text("Selected Block: %s", block.c_str());

    ImGui::End();
}