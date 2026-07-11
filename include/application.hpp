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

class Application {
    public:
        int Run();
    private:
        std::unique_ptr<Window> m_window;
        std::unique_ptr<ImGuiHandler> m_imgui;

        void renderGUI(bool showDebug, Camera& camera);
};