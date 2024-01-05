#include "pch.h"
#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(std::string path)
{
    m_data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);

    if (!m_data) {
        std::cout << "Failed to load image: " << path;
        exit(EXIT_FAILURE);
    }
    std::cout << "Load Texture: " << path << '\n';
}

Texture::~Texture()
{
    stbi_image_free(m_data);
}

uint8_t* Texture::GetRGBA(const glm::vec2& uv)
{
    int x = static_cast<int>(uv.x * m_width);
    int y = static_cast<int>(uv.y * m_height);

    int index = (x + m_width * y) * m_channels;

    return m_data + index;
}
