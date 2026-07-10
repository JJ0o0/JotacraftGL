#include <graphics/mesh.hpp>
#include <glad/gl.h>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
    : m_indexCount(indices.size()) {
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // TexCoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &m_ebo);
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void Mesh::Draw() const {
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
}