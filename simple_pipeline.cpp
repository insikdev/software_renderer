#include "pch.h"
#include "simple_pipeline.h"
#include "texture.h"
#include "plane.h"

void SimplePipeline::VertexShader(std::vector<Vertex>& vertices)
{
    glm::mat4 transpose = glm::transpose(m_uniform.proj);
    std::array<Plane, 6> frustumPlanes = {
        Plane(-(transpose[3] - transpose[1])), //+Y 0
        Plane(-(transpose[3] + transpose[1])), //-Y 1
        Plane(-(transpose[3] - transpose[0])), //+X 2
        Plane(-(transpose[3] + transpose[0])), //-X 3
        Plane(-(transpose[3] - transpose[2])), //+Z 4
        Plane(-(transpose[3] + transpose[2])), //-Z 5
    };

    for (Vertex& v : vertices) {
        for (auto& plane : frustumPlanes) {
            float d = plane.DistanceFromPoint(m_uniform.view * m_uniform.world * v.position);
            if (d > 0) {
                std::cout << "frustum culling\n";
            }
        }
    }

    for (Vertex& v : vertices) {
        v.position = m_uniform.proj * m_uniform.view * m_uniform.world * v.position;
    }
}

void SimplePipeline::FragmentShader()
{
    for (uint32_t i = 0; i < m_totalPixels; i++) {
        if (m_fragments[i].active) {
            uint8_t* color = m_uniform.texture->GetRGBA(m_fragments[i].uv);
            m_pixels[i] = SDL_MapRGBA(m_format, *color, *(color + 1), *(color + 2), 255);
        }
    }
}
