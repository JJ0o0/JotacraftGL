#pragma once

#include <GLFW/glfw3.h>

class ImGuiHandler {
    public:
        void Init(GLFWwindow* window);
        void Destroy();

        void Begin();
        void End();
    private:
        void setTheme();
};