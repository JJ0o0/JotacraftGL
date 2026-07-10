#include <graphics/texture.hpp>
#include <iostream>
#include <stb/stb_image.h>

Texture::Texture(const TextureProperties& properties) : m_properties(properties) {
    glGenTextures(1, &m_id);

    ChangeImage(m_properties.ImagePath);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_id);
}

void Texture::Bind(int unit) const { 
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::Unbind() const { 
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::ChangeImage(const std::string& filepath) {
    if (filepath.empty()) {
        std::cerr << "File path is empty or invalid!\n";
        return;
    }

    Bind(m_properties.Unit);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, toGLenum(m_properties.Wrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, toGLenum(m_properties.Wrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toGLenum(m_properties.MinFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toGLenum(m_properties.MagFilter));

    int width, height, numChannels;
    uint8_t* data = stbi_load(
        filepath.c_str(),
        &width, &height, &numChannels,
        0
    );

    if (!data) {
        std::cerr << "Could not load texture at \"" << filepath << "\"\n";
        stbi_image_free(data);
        return;
    }

    m_properties.Id = static_cast<int>(m_id);
    m_properties.Width = width;
    m_properties.Height = height;
    m_properties.Channels = numChannels;
    m_properties.Mipmaps = m_properties.MinFilter != TextureFilterOption::Linear && m_properties.MinFilter != TextureFilterOption::Nearest;

    GLenum format = GL_RGB;

    switch (numChannels) {
        case 1: format = GL_RED; break;
        case 2: format = GL_RG; break;
        case 4: format = GL_RGBA; break;
    }

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        width, height, 0,
        format,
        GL_UNSIGNED_BYTE,
        data
    );

    if (m_properties.Mipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);
    m_properties.ImagePath = filepath;
}

void Texture::ChangeWrap(TextureWrapOption option) {
    m_properties.Wrap = option;

    Bind(m_properties.Unit);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, toGLenum(m_properties.Wrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, toGLenum(m_properties.Wrap));
}

void Texture::ChangeMinFilter(TextureFilterOption option) {
    m_properties.MinFilter = option;
    m_properties.Mipmaps = m_properties.MinFilter != TextureFilterOption::Linear && m_properties.MinFilter != TextureFilterOption::Nearest;

    Bind(m_properties.Unit);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toGLenum(m_properties.MinFilter));
}

void Texture::ChangeMagFilter(TextureFilterOption option) {
    m_properties.MagFilter = option;

    Bind(m_properties.Unit);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toGLenum(m_properties.MagFilter));
}