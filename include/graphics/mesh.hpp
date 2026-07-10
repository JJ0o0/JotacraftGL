#pragma once

#include "vertex.hpp"
#include <cstdint>
#include <vector>

class Mesh {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
        ~Mesh();
        
        void Draw() const;
    private:
        uint32_t m_vao = 0;
        uint32_t m_vbo = 0;
        uint32_t m_ebo = 0;

        size_t m_indexCount = 0;
};