#pragma once

class Texture {
public:
    Texture(std::string path);
    ~Texture();

public:
    uint8_t* GetRGBA(float u, float v);
    uint8_t* GetRGBA(const glm::vec2& uv);

private:
    int m_width;
    int m_height;
    int m_channels;
    uint8_t* m_data;
};
