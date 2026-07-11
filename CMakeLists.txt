cmake_minimum_required(VERSION 3.20)

project(
    Jotacraft
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_subdirectory(
    "./external"
    "${CMAKE_BINARY_DIR}/external"
)

add_executable(${CMAKE_PROJECT_NAME}
    "src/main.cpp"
    "src/application.cpp"
    "src/camera.cpp"
    "src/player.cpp"

    "src/platform/window.cpp"
    "src/platform/imgui_handler.cpp"

    "src/graphics/mesh.cpp"
    "src/graphics/mesher.cpp"
    "src/graphics/shader.cpp"
    "src/graphics/texture.cpp"

    "src/world/chunk.cpp"
    "src/world/world.cpp"
    "src/world/block_registry.cpp"

    "src/external/gl.c"
    "src/external/stb_image.cpp"
)

target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE
        "./include"
        "./external"
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
        OpenGL::GL
        glfw
        glm::glm
        imgui
)