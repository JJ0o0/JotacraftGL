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

void Window::mouseScrollCallback(GLFWwindow* glfwWindow, double xoffset, double yoffset) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

    if (window->OnMouseScroll) window->OnMouseScroll(xoffset, yoffset);
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
    glfwSetScrollCallback(m_handle, mouseScrollCallback);
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
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glViewport(0, 0, m_properties.Width, m_properties.Height);
    glfwSwapInterval(m_properties.VSync);

    SetIcon(m_properties.IconPath);

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

void Window::ToggleVSync() {
    m_properties.VSync = !m_properties.VSync;
    glfwSwapInterval(m_properties.VSync);
}

void Window::SetTitle(const std::string& title) {
    glfwSetWindowTitle(m_handle, title.c_str());
    m_properties.Title = title;
}

void Window::SetIcon(const std::string& path) {
    if (path.empty()) {
        glfwSetWindowIcon(m_handle, 0, nullptr);
        m_properties.IconPath.clear();
        return;
    }

    GLFWimage icon{};

    int channels = 0;
    icon.pixels = stbi_load(path.c_str(), &icon.width, &icon.height, &channels, 4);

    if (!icon.pixels) {
        std::cout << "Failed to load window icon at \"" << path << "\": " << (stbi_failure_reason() ? stbi_failure_reason() : "Unknown") << "\n";
        return;
    }

    glfwSetWindowIcon(m_handle, 1, &icon);
    stbi_image_free(icon.pixels);

    m_properties.IconPath = path;
}