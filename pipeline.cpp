#include "pch.h"
#include "pipeline.h"
#include "object.h"

void Pipeline::Draw(const Object* obj)
{
    std::vector<Vertex> vertices(obj->m_mesh->vertices);
    const std::vector<uint32_t>& indices = obj->m_mesh->indices;

    m_uniform.world = obj->m_transform.GetWorldMatrix();
    m_uniform.texture = obj->m_texture;

    VertexShader(vertices);

    for (int i = 0; i < indices.size(); i += 3) {
        Rasterizer({ vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]] });
    }

    FragmentShader();
}

// viewport transform
// origin of screen space is top-left
// ndcX : [-1, 1] -> screenX : [-0.5, width - 0.5]
// ndcY : [-1, 1] -> screenY : [-0.5, height - 0.5]
glm::uvec2 Pipeline::NDCToScreen(const glm::vec3& ndc)
{
    float aspectRatio = static_cast<float>(m_width) / m_height;

    float screenX = (ndc.x / aspectRatio + 1.0f) * m_width * 0.5f - 0.5f;
    float screenY = (1.0f - ndc.y) * m_height * 0.5f - 0.5f;

    return glm::uvec2(static_cast<uint32_t>(screenX), static_cast<uint32_t>(screenY));
}

glm::vec3 Pipeline::CalculateBarycentricCoordinate(const glm::vec2& target, const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2)
{
    glm::vec2 w = target - p2;
    glm::vec2 u = p0 - p2;
    glm::vec2 v = p1 - p2;

    float wdotu = glm::dot(w, u);
    float wdotv = glm::dot(w, v);
    float udotv = glm::dot(u, v);
    float udotu = glm::dot(u, u);
    float vdotv = glm::dot(v, v);

    float denom = udotv * udotv - udotu * vdotv;

    float s = (wdotv * udotv - wdotu * vdotv) / denom;
    float t = (wdotu * udotv - wdotv * udotu) / denom;

    return glm::vec3(s, t, 1.0f - s - t);
}

void Pipeline::Rasterizer(const std::array<Vertex, 3>& vertices)
{
    // perspective division
    glm::vec3 ndc0 = glm::vec3(vertices[0].position / vertices[0].position.w);
    glm::vec3 ndc1 = glm::vec3(vertices[1].position / vertices[1].position.w);
    glm::vec3 ndc2 = glm::vec3(vertices[2].position / vertices[2].position.w);

    // viewport transform
    glm::uvec2 p0 = NDCToScreen(ndc0);
    glm::uvec2 p1 = NDCToScreen(ndc1);
    glm::uvec2 p2 = NDCToScreen(ndc2);

    int minX = std::min({ p0.x, p1.x, p2.x });
    minX = std::max(0, minX);
    int maxX = std::max({ p0.x, p1.x, p2.x });
    maxX = std::min(m_width - 1, maxX);

    int minY = std::min({ p0.y, p1.y, p2.y });
    minY = std::max(0, minY);
    int maxY = std::max({ p0.y, p1.y, p2.y });
    maxY = std::min(m_height - 1, maxY);

    for (int y = minY; y < maxY; y++) {
        for (int x = minX; x < maxX; x++) {
            int index = x + y * m_width;
            glm::vec2 pixel = glm::vec2(static_cast<float>(x), static_cast<float>(y));

            glm::vec3 bc = CalculateBarycentricCoordinate(pixel, p0, p1, p2);

            if ((bc.x >= 0.0f) && (bc.x <= 1.0f) && (bc.y >= 0.0f) && (bc.y <= 1.0f) && (bc.z >= 0.0f) && (bc.z <= 1.0f)) {
                float depth = bc.x * ndc0.z + bc.y * ndc1.z + bc.z * ndc2.z;

                // depth test
                if (m_fragments[index].active && depth > m_fragments[index].depth) {
                    continue;
                }

                float invZ0 = 1.0f / vertices[0].position.w;
                float invZ1 = 1.0f / vertices[1].position.w;
                float invZ2 = 1.0f / vertices[2].position.w;
                float invZ = 1.0f / (invZ0 * bc.x + invZ1 * bc.y + invZ2 * bc.z);

                // interpolation using barycentric coordinate
                m_fragments[index].active = true;
                m_fragments[index].depth = depth;
                m_fragments[index].uv = (bc.x * vertices[0].uv * invZ0 + bc.y * vertices[1].uv * invZ1 + bc.z * vertices[2].uv * invZ2) * invZ;
            }
        }
    }
}
