#pragma once

#include <functional>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <string>

struct WindowProperties {
    std::string Title = "Window";
    std::string IconPath = "assets/icon.png";
    uint32_t Width = 800;
    uint32_t Height = 600;
    bool VSync = true;
};

class Window {
    public:
        Window(const WindowProperties& properties = {});
        ~Window();

        void SwapBuffers() const { glfwSwapBuffers(m_handle); }
        void PollEvents() const { glfwPollEvents(); }
        void Close() const { glfwSetWindowShouldClose(m_handle, true); }
        bool ShouldClose() const { return glfwWindowShouldClose(m_handle); }

        bool IsMouseLocked() const { return glfwGetInputMode(m_handle, GLFW_CURSOR) == GLFW_CURSOR_DISABLED; }
        void ToggleMouseLock();
        void ToggleVSync();

        void SetTitle(const std::string& title);
        void SetIcon(const std::string& path);

        std::function<void(int)> WasKeyPressed;
        std::function<void(int)> WasMouseButtonPressed;
        std::function<void(double, double)> OnMouseMove;
        std::function<void(double, double)> OnMouseScroll;

        const WindowProperties& GetProperties() const { return m_properties; }
        GLFWwindow* GetHandle() const { return m_handle; }
    private:
        WindowProperties m_properties;
        GLFWwindow* m_handle;

        static void keyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods);
        static void mouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods);
        static void mouseCallback(GLFWwindow* glfwWindow, double xpos, double ypos);
        static void mouseScrollCallback(GLFWwindow* glfwWindow, double xoffset, double yoffset);
};