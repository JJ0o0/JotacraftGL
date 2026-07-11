#include <application.hpp>


int Application::Run() {
    m_window = std::make_unique<Window>(WindowProperties{ 
        .Title = "Jotacraft" 
    });

    m_imgui = std::make_unique<ImGuiHandler>();
    m_imgui->Init(m_window->GetHandle());

    Player player{};
    World world{};
    BlockRegistry::Init();

    Shader shader{"assets/shaders/default.vert", "assets/shaders/default.frag"};
    AtlasTexture atlas{};

    std::vector<Mesh> chunkMeshes;
    for (int x = -2; x < 2; x++) {
        for (int z = -2; z < 2; z++) {
            world.GenerateChunk({x, z}, 6);
        }
    }

    for (int x = -2; x < 2; x++) {
        for (int z = -2; z < 2; z++) {
            MeshData data = Mesher::GenerateMesh(world, {x, z});
            chunkMeshes.push_back({data.vertices, data.indices});
        }
    }

    bool showDebug = false;
    m_window->WasKeyPressed = [&](int key) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                if (!m_window->IsMouseLocked()) player.GetCamera().ResetMouseMovement();

                m_window->ToggleMouseLock();

                break;
            case GLFW_KEY_F1:
                showDebug = !showDebug;
                break;
        }
    };

    m_window->WasMouseButtonPressed = [&](int button) {
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                break;
        }
    };

    m_window->OnMouseMove = [&](double xpos, double ypos) {
        if (m_window->IsMouseLocked()) player.GetCamera().HandleMouseMovement(xpos, ypos);
    };

    glm::mat4 model{1.0f};

    float lastTime = glfwGetTime();
    while (!m_window->ShouldClose()) {
        float currTime = glfwGetTime();
        float deltaTime = currTime - lastTime;
        lastTime = currTime;

        m_window->PollEvents();

        player.HandleMovement(m_window->GetHandle(), deltaTime);
        player.Update(world, deltaTime);

        glClearColor(0.5294f, 0.8078f, 0.9215f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Bind();
            shader.SetMat4("uModel", model);
            shader.SetMat4("uView", player.GetCamera().GetViewMatrix(player.GetEyePosition()));
            shader.SetMat4("uProjection", player.GetCamera().GetProjectionMatrix());
            shader.SetInt("uTexture", 0);

            atlas.Bind();
                for (auto& mesh : chunkMeshes) mesh.Draw();
            atlas.Unbind();
        shader.Unbind();

        m_imgui->Begin();
            renderGUI(showDebug, player);
        m_imgui->End();

        m_window->SwapBuffers();
    }

    m_imgui->Destroy();
    return 0;
}

void Application::renderGUI(bool showDebug, Player& player) { 
    if (!showDebug) return;
    
    ImGui::SetNextWindowPos({15, 15});
    ImGui::SetNextWindowSize({300, 100});
    ImGui::Begin("Debug", nullptr,
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize
    );

    auto pos = player.GetPosition();
    auto vel = player.GetVelocity();
    ImGui::Text("Player Position: x %.2f, y %.2f, z %.2f", pos.x, pos.y, pos.z);
    ImGui::Text("Player Velocity: x %.2f, y %.2f, z %.2f", vel.x, vel.y, vel.z);

    ImGui::End();
}
