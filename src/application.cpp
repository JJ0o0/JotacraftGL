#include <application.hpp>
#include <game.hpp>

int Application::Run() {
    m_window = std::make_unique<Window>(WindowProperties{ 
        .Title = "Jotacraft" 
    });

    m_imgui = std::make_unique<ImGuiHandler>();
    m_imgui->Init(m_window->GetHandle());

    Game game(*m_window);
    game.Initialize();

    float lastTime = glfwGetTime();
    while (!m_window->ShouldClose()) {
        float currTime = glfwGetTime();
        float deltaTime = currTime - lastTime;
        lastTime = currTime;

        deltaTime = std::min(deltaTime, 0.05f);

        m_window->PollEvents();

        game.Update(deltaTime);

        glClearColor(0.5294f, 0.8078f, 0.9215f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        game.Render();

        m_imgui->Begin();
            game.RenderDebugGUI();
        m_imgui->End();

        m_window->SwapBuffers();
    }

    m_imgui->Destroy();
    return 0;
}