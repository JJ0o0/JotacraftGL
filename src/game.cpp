#include <game.hpp>

#include <platform/system_info.hpp>

#include <world/block_registry.hpp>
#include <world/world_interaction.hpp>

#include <graphics/light_engine.hpp>
#include <graphics/hud/gui_registry.hpp>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <algorithm>

Game::Game(Window& window) 
    : m_window(window) {}

void Game::Initialize() {
    BlockRegistry::Init();
    BlockIconRegistry::Init();
    GUIRegistry::Init();

    m_window.WasKeyPressed = [&](int key) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                if (!m_window.IsMouseLocked()) m_player.GetCamera().ResetMouseMovement();

                m_window.ToggleMouseLock();

                break;
            case GLFW_KEY_V: m_window.ToggleVSync(); break;

            case GLFW_KEY_1: m_selectedSlot = 0; break;
            case GLFW_KEY_2: m_selectedSlot = 1; break;
            case GLFW_KEY_3: m_selectedSlot = 2; break;
            case GLFW_KEY_4: m_selectedSlot = 3; break;
            case GLFW_KEY_5: m_selectedSlot = 4; break;
            case GLFW_KEY_6: m_selectedSlot = 5; break;
            case GLFW_KEY_7: m_selectedSlot = 6; break;
            case GLFW_KEY_8: m_selectedSlot = 7; break;
            case GLFW_KEY_9: m_selectedSlot = 8; break;

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
                WorldInteraction::PlaceBlock(m_world, m_player, m_worldRenderer, target, GetSelectedBlock());
                break;
        }
    };

    m_window.OnMouseMove = [&](double xpos, double ypos) {
        if (m_window.IsMouseLocked()) m_player.GetCamera().HandleMouseMovement(xpos, ypos);
    };

    m_window.OnMouseScroll = [&](double xoffset, double yoffset) {
        if (m_window.IsMouseLocked()) {
            if (yoffset > 0) { 
                if (m_selectedSlot + 1 > 8) m_selectedSlot = 0;
                else m_selectedSlot++;
            }
            else if (yoffset < 0) {
                if (m_selectedSlot - 1 < 0) m_selectedSlot = 8;
                else m_selectedSlot--;
            }

            m_selectedSlot = std::clamp(m_selectedSlot, 0, 8);
        }
    };

    auto initResult = m_world.Update(m_player.GetPosition());
    m_worldRenderer.Update(m_world, initResult);

    m_player.SetPosition({0, m_world.GetSurfaceHeight(0, 3), 3});
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

    m_guiRenderer.Render(
        m_worldRenderer.GetAtlas(),
        static_cast<int>(m_window.GetProperties().Width), 
        static_cast<int>(m_window.GetProperties().Height),
        m_selectedSlot
    );
}

void Game::RenderDebugGUI(float fps) {
    if (!m_showDebug) return;
    
    ImGui::SetNextWindowPos({15, 15});
    ImGui::SetNextWindowSize({300, 270});
    ImGui::Begin("Debug", nullptr,
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize
    );

    auto pos = m_player.GetPosition();
    auto vel = m_player.GetVelocity();
    auto memory = SystemInfo::GetMemoryUsage() / (1024.0f * 1024.0f);

    ImGui::Text("F3 to open or close\n\n");

    ImGui::Text("Performance");
    ImGui::Separator();

    ImGui::Text("FPS: %.0f", fps);
    ImGui::Text("Frame Time: %.2f ms", 1000.0f / fps);
    ImGui::Text("VSync: %s", m_window.GetProperties().VSync ? "On" : "Off");
    ImGui::Text("Memory: %.2fmb", memory);

    ImGui::Text("\nPlayer");
    ImGui::Separator();

    ImGui::Text("Position:");
    ImGui::InputFloat3("##Position", &pos.x, "%.2f", ImGuiInputTextFlags_ReadOnly);

    ImGui::Text("Velocity:");
    ImGui::InputFloat3("##Velocity", &vel.x, "%.2f", ImGuiInputTextFlags_ReadOnly);

    ImGui::End();
}