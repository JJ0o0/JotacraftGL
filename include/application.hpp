#pragma once

#include <platform/window.hpp>
#include <platform/imgui_handler.hpp>
#include <graphics/shader.hpp>
#include <graphics/texture.hpp>
#include <graphics/mesh.hpp>
#include <graphics/shapes/cube.hpp>
#include <camera.hpp>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <memory>

static const char* wrapNames[] = {
    "Repeat",
    "Mirrored Repeat",
    "Clamp To Edge"
};

constexpr TextureWrapOption wrapOptions[] = {
    TextureWrapOption::Repeat,
    TextureWrapOption::MirroredRepeat,
    TextureWrapOption::ClampToEdge
};

static const char* minFilterNames[] = {
    "Nearest",
    "Linear",
    "Nearest Mipmap Nearest",
    "Linear Mipmap Nearest",
    "Nearest Mipmap Linear",
    "Linear Mipmap Linear"
};

constexpr TextureFilterOption filterOptions[] = {
    TextureFilterOption::Nearest,
    TextureFilterOption::Linear,
    TextureFilterOption::NearestMipmapNearest,
    TextureFilterOption::LinearMipmapNearest,
    TextureFilterOption::NearestMipmapLinear,
    TextureFilterOption::LinearMipmapLinear,
};

static const char* magFilterNames[] = {
    "Nearest",
    "Linear"
};

struct GuiState {
    std::string filepath = "assets/textures/default.jpg";

    int currentWrap = 0;
    int currentMin = 5;
    int currentMag = 1;
};

class Application {
    public:
        int Run();
    private:
        std::unique_ptr<Window> m_window;
        std::unique_ptr<ImGuiHandler> m_imgui;

        void renderGUI(GuiState& state, Texture& texture);
};