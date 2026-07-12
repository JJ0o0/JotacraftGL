#include <platform/window.hpp>
#include <stb/stb_image.h>
#include <iostream>

void Window::keyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

    switch (action) {
        case GLFW_PRESS:
            if (window->WasKeyPressed) window->WasKeyPressed(key);
            break;
    }
}

void Window::mouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

    switch (action) {
        case GLFW_PRESS:
            if (window->WasMouseButtonPressed) window->WasMouseButtonPressed(button);
            break;
    }
}

void Window::mouseCallback(GLFWwindow* glfwWindow, double xpos, double ypos) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

    if (window->OnMouseMove) window->OnMouseMove(xpos, ypos);
}

Window::Window(const WindowProperties& properties)
    : m_properties(properties) {
    if (!glfwInit()) {
        std::cerr << "Could not initialize GLFW.\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_handle = glfwCreateWindow(
        m_properties.Width, m_properties.Height, 
        m_properties.Title.c_str(),
        nullptr, nullptr
    );

    if (!m_handle) {
        std::cerr << "Could not create Window.\n";
        glfwTerminate();
        return;
    }

    glfwSetWindowUserPointer(m_handle, this);
    glfwSetKeyCallback(m_handle, keyCallback);
    glfwSetMouseButtonCallback(m_handle, mouseButtonCallback);
    glfwSetCursorPosCallback(m_handle, mouseCallback);
    glfwMakeContextCurrent(m_handle);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Could not initialize OpenGL.\n";
        glfwDestroyWindow(m_handle);
        glfwTerminate();
        return;
    }
    
    glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glViewport(0, 0, m_properties.Width, m_properties.Height);
    glfwSwapInterval(1);

    stbi_set_flip_vertically_on_load(true);

    std::cout << "Created window with " << m_properties.Width << "x" << m_properties.Height << ".\n"; 
    std::cout << "Initialized OpenGL context.\n"; 
}

Window::~Window() {
    std::cout << "Destroying Window...\n";
    glfwDestroyWindow(m_handle);
    glfwTerminate();
}

void Window::ToggleMouseLock() {
    if (!IsMouseLocked()) glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}