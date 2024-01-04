#include "pch.h"
#include "simple_pipeline.h"
#include "texture.h"
#include "plane.h"

void SimplePipeline::VertexShader(std::vector<Vertex>& vertices)
{
    for (Vertex& v : vertices) {
        v.position = m_uniform.proj * m_uniform.view * m_uniform.world * v.position;
    }
}

void SimplePipeline::FragmentShader()
{
    for (uint32_t i = 0; i < m_totalPixels; i++) {
        if (m_fragments[i].active) {
            if (IsWireFrameMode) {
                m_pixels[i] = SDL_MapRGBA(m_format, 255, 255, 255, 255);
            } else {
                uint8_t* color = m_uniform.texture->GetRGBA(m_fragments[i].uv);
                m_pixels[i] = SDL_MapRGBA(m_format, *color, *(color + 1), *(color + 2), 255);
            }
        }
    }
}
