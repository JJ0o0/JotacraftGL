#pragma once

#include <graphics/vertex.hpp>
#include <glad/gl.h>
#include <cstdint>
#include <vector>

enum class DrawMode {
    Triangles = GL_TRIANGLES,
    Lines = GL_LINES
};

class Mesh {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
        ~Mesh();
        
        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;
        Mesh(Mesh&& other) noexcept;
        Mesh& operator=(Mesh&& other) noexcept;

        void Draw(DrawMode mode = DrawMode::Triangles) const;
    private:
        uint32_t m_vao = 0;
        uint32_t m_vbo = 0;
        uint32_t m_ebo = 0;

        size_t m_indexCount = 0;
};