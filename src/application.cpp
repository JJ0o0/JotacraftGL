#include <application.hpp>

int Application::Run() {
    m_window = std::make_unique<Window>(WindowProperties{ 
        .Title = "Texture Laboratory" 
    });

    m_imgui = std::make_unique<ImGuiHandler>();
    m_imgui->Init(m_window->GetHandle());

    GuiState guiState{};
    Camera camera{};

    Shader shader{"assets/shaders/default.vert", "assets/shaders/default.frag"};
    Texture texture{TextureProperties{
        .ImagePath = guiState.filepath
    }};

    Mesh mesh = CreateCube();

    m_window->WasKeyPressed = [&](int key) {
        switch (key) {
            case GLFW_KEY_ESCAPE: m_window->Close(); break;
        }
    };

    m_window->OnMouseMove = [&](double xpos, double ypos) {
        camera.HandleMouseMovement(xpos, ypos);
    };

    glm::mat4 model{1.0f};

    float lastTime = glfwGetTime();
    while (!m_window->ShouldClose()) {
        float currTime = glfwGetTime();
        float deltaTime = currTime - lastTime;
        lastTime = currTime;

        m_window->PollEvents();

        camera.HandleMovementInput(m_window->GetHandle(), deltaTime);
        camera.HandleMouseInput(m_window->GetHandle());

        glClearColor(0.1f, 0.1f, 0.13f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Bind();
            shader.SetMat4("uModel", model);
            shader.SetMat4("uView", camera.GetViewMatrix());
            shader.SetMat4("uProjection", camera.GetProjectionMatrix());
            shader.SetInt("uTexture", 0);

            texture.Bind();
                mesh.Draw();
            texture.Unbind();
        shader.Unbind();

        m_imgui->Begin();
            renderGUI(guiState, texture);
        m_imgui->End();

        m_window->SwapBuffers();
    }

    m_imgui->Destroy();
    return 0;
}

void Application::renderGUI(GuiState& state, Texture& texture) {
    ImGui::SetNextWindowSize(ImVec2{ 320, 220});
    ImGui::Begin("Settings", nullptr, 
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse
    );

    ImGui::InputText("Image Path", &state.filepath);

    const TextureProperties& props = texture.GetProperties();
    ImGui::Text("Resolution: %dx%d", props.Width, props.Height);
    ImGui::Text("Channels: %d", props.Channels);
    ImGui::Text("Mipmaps: %s", props.Mipmaps ? "Yes" : "No");
    ImGui::Text("Render Id: %d", props.Id);
    
    if (ImGui::Combo("Wrap", &state.currentWrap, wrapNames, IM_ARRAYSIZE(wrapNames))) {
        texture.ChangeWrap(wrapOptions[state.currentWrap]);
    }

    if (ImGui::Combo("Min Filter", &state.currentMin, minFilterNames, IM_ARRAYSIZE(minFilterNames))) {
        texture.ChangeMinFilter(filterOptions[state.currentMin]);
    }

    if (ImGui::Combo("Mag Filter", &state.currentMag, magFilterNames, IM_ARRAYSIZE(magFilterNames))) {
        texture.ChangeMagFilter(filterOptions[state.currentMag]);
    }

    if (ImGui::Button("Reload Texture")) {
        texture.ChangeImage(state.filepath);
    }

    ImGui::End();
}
