#pragma once

#include <functional>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <string>

struct WindowProperties {
    std::string Title = "Shader Playground";
    uint32_t Width = 800;
    uint32_t Height = 600;
};

class Window {
    public:
        Window(const WindowProperties& properties = {});
        ~Window();

        void SwapBuffers() const { glfwSwapBuffers(m_handle); }
        void PollEvents() const { glfwPollEvents(); }
        void Close() const { glfwSetWindowShouldClose(m_handle, true); }
        bool ShouldClose() const { return glfwWindowShouldClose(m_handle); }

        std::function<void(int)> WasKeyPressed;
        std::function<void(double, double)> OnMouseMove;

        const WindowProperties& GetProperties() const { return m_properties; }
        GLFWwindow* GetHandle() const { return m_handle; }
    private:
        WindowProperties m_properties;
        GLFWwindow* m_handle;

        static void keyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods);
        static void mouseCallback(GLFWwindow* glfwWindow, double xpos, double ypos);
};