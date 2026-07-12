#pragma once

#include <graphics/vertex.hpp>
#include <glad/gl.h>
#include <cstdint>
#include <vector>

class Mesh2D {
    public:
        Mesh2D(const std::vector<Vertex2D>& vertices, const std::vector<uint32_t>& indices);
        ~Mesh2D();
        
        Mesh2D(const Mesh2D&) = delete;
        Mesh2D& operator=(const Mesh2D&) = delete;
        Mesh2D(Mesh2D&& other) noexcept;
        Mesh2D& operator=(Mesh2D&& other) noexcept;

        void Draw() const;
    private:
        uint32_t m_vao = 0;
        uint32_t m_vbo = 0;
        uint32_t m_ebo = 0;

        size_t m_indexCount = 0;
};