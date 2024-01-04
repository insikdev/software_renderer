#include "pch.h"
#include "pipeline.h"
#include "object.h"
#include "frustum.h"

void Pipeline::Draw(const Object* obj)
{
    std::vector<Vertex> vertices(obj->m_mesh->vertices);
    const std::vector<uint32_t>& indices = obj->m_mesh->indices;

    m_uniform.world = obj->m_transform.GetWorldMatrix();
    m_uniform.texture = obj->m_texture;

    // glm::vec3 min = glm::vec3(0.0f);
    // glm::vec3 max = glm::vec3(0.0f);

    // for (const auto& v : vertices) {
    //     min.x = glm::min(min.x, v.position.x);
    //     min.y = glm::min(min.y, v.position.y);
    //     min.z = glm::min(min.z, v.position.z);

    //    max.x = glm::max(max.x, v.position.x);
    //    max.y = glm::max(max.y, v.position.y);
    //    max.z = glm::max(max.z, v.position.z);
    //}

    // if (FrustumCulling(m_uniform.proj * m_uniform.view * m_uniform.world)) {
    //     return;
    // }

    VertexShader(vertices);

    for (int i = 0; i < indices.size(); i += 3) {
        Rasterizer({ vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]] });
    }

    FragmentShader();
}

int Pipeline::ComputeOutCode(const glm::ivec2& input, const glm::ivec2& min, const glm::ivec2& max)
{
    int code = 0;

    if (input.x < min.x) {
        code |= 0b0001;
    } else if (input.x > max.x) {
        code |= 0b0010;
    }

    if (input.y < min.y) {
        code |= 0b0100;
    } else if (input.y > max.y) {
        code |= 0b1000;
    }

    return code;
}

bool Pipeline::LineClip(glm::ivec2& p0, glm::ivec2& p1, const glm::ivec2& min, const glm::ivec2& max)
{
    int code0 = ComputeOutCode(p0, min, max);
    int code1 = ComputeOutCode(p1, min, max);

    int dx = p1.x - p0.x;
    int dy = p1.y - p0.y;

    while (true) {
        if ((code0 | code1) == 0) {
            return true;
        }

        if ((code0 & code1) != 0) {
            return false;
        }

        int currentCode = code0 != 0 ? code0 : code1;
        glm::vec2 clippedPoint(0);

        if (currentCode < 0b0100) {
            if (currentCode & 0b0001) {
                clippedPoint.x = min.x;
            } else {
                clippedPoint.x = max.x;
            }

            clippedPoint.y = p0.y + dy * (clippedPoint.x - p0.x) / dx;
        } else {
            if (currentCode & 0b0100) {
                clippedPoint.y = min.y;
            } else {
                clippedPoint.y = max.y;
            }

            clippedPoint.x = p0.x + dx * (clippedPoint.y - p0.y) / dy;
        }

        if (currentCode == code0) {
            p0 = clippedPoint;
            code0 = ComputeOutCode(p0, min, max);
        } else {
            p1 = clippedPoint;
            code1 = ComputeOutCode(p1, min, max);
        }
    }

    return false;
}

void Pipeline::DrawLine(glm::ivec2 p0, glm::ivec2 p1)
{
    if (!LineClip(p0, p1, glm::ivec2(0, 0), glm::ivec2(m_width - 1, m_height - 1))) {
        return;
    }

    int w = p1.x - p0.x;
    int h = p1.y - p0.y;

    bool isGraudalSlope = std::abs(w) >= std::abs(h);
    int dx = w >= 0 ? 1 : -1;
    int dy = h >= 0 ? 1 : -1;

    int fw = dx * w;
    int fh = dy * h;

    int f = isGraudalSlope ? 2 * fh - fw : 2 * fw - fh;
    int f1 = isGraudalSlope ? 2 * fh : 2 * fw;
    int f2 = isGraudalSlope ? 2 * (fh - fw) : 2 * (fw - fh);

    int x = p0.x;
    int y = p0.y;

    if (isGraudalSlope) {
        while (x != p1.x) {
            int index = x + y * m_width;
            if (index > 0 && index < m_height * m_width) {
                m_fragments[index].active = true;
            }

            if (f < 0) {
                f += f1;
            } else {
                f += f2;
                y += dy;
            }
            x += dx;
        }
    } else {
        while (y != p1.y) {
            int index = x + y * m_width;
            if (index > 0 && index < m_height * m_width) {
                m_fragments[index].active = true;
            }

            if (f < 0) {
                f += f1;
            } else {
                f += f2;
                x += dx;
            }
            y += dy;
        }
    }
}

bool Pipeline::FrustumCulling(const glm::mat4& mvp)
{
    glm::vec3 center = glm::vec3(0.0f);
    float radius = glm::sqrt(3);

    glm::mat4 transpose = glm::transpose(mvp);
    std::array<Plane, 6> frustumPlanes = {
        Plane(-(transpose[3] - transpose[1])), //+Y 0
        Plane(-(transpose[3] + transpose[1])), //-Y 1
        Plane(-(transpose[3] - transpose[0])), //+X 2
        Plane(-(transpose[3] + transpose[0])), //-X 3
        Plane(-(transpose[3] - transpose[2])), //+Z 4
        Plane(-(transpose[3] + transpose[2])), //-Z 5
    };

    glm::vec3 min = glm::vec3(-1.0f, -1.0f, -1.0f);
    glm::vec3 max = glm::vec3(1.0f, 1.0f, 1.0f);

    // for (auto& plane : frustumPlanes) {
    //     float d = plane.DistanceFromPoint(center);
    //     if (d > radius) {
    //         std::cout << "cull\n";
    //         return true;
    //     }
    // }

    for (auto plane : frustumPlanes) {
        glm::vec3 p = min;
        glm::vec3 n = max;
        if (plane.m_normal.x >= 0.0f) {
            p.x = max.x;
            n.x = min.x;
        }
        if (plane.m_normal.y >= 0.0f) {
            p.y = max.y;
            n.y = min.y;
        }
        if (plane.m_normal.z >= 0.0f) {
            p.z = max.z;
            n.z = min.z;
        }

        if (plane.DistanceFromPoint(n) > 0.0f) {
            std::cout << "cull\n";
            return true;
        }
    }

    return false;
}

void Pipeline::Rasterizer(const std::array<Vertex, 3>& vertices)
{
    // clipping
    if (vertices[0].position.z < -vertices[0].position.w) {
        std::cout << "clip\n";
    }

    // perspective division
    // clip coordinate -> NDC
    glm::vec3 ndc0 = glm::vec3(vertices[0].position / vertices[0].position.w);
    glm::vec3 ndc1 = glm::vec3(vertices[1].position / vertices[1].position.w);
    glm::vec3 ndc2 = glm::vec3(vertices[2].position / vertices[2].position.w);

    // viewport transform
    // NDC -> screen coordinate
    glm::ivec2 p0 = NDCToScreen(ndc0);
    glm::ivec2 p1 = NDCToScreen(ndc1);
    glm::ivec2 p2 = NDCToScreen(ndc2);

    if (IsWireFrameMode) {
        DrawLine(p0, p1);
        DrawLine(p1, p2);
        DrawLine(p2, p0);
        return;
    }

    // calculate bounding box
    int minX = std::max(0, std::min({ p0.x, p1.x, p2.x }));
    int maxX = std::min(m_width, std::max({ p0.x, p1.x, p2.x }));
    int minY = std::max(0, std::min({ p0.y, p1.y, p2.y }));
    int maxY = std::min(m_height, std::max({ p0.y, p1.y, p2.y }));

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

// viewport transform
// origin of screen space is top-left
// ndcX : [-1, 1] -> screenX : [-0.5, width - 0.5]
// ndcY : [-1, 1] -> screenY : [-0.5, height - 0.5]
glm::ivec2 Pipeline::NDCToScreen(const glm::vec3& ndc)
{
    float aspectRatio = static_cast<float>(m_width) / m_height;

    float screenX = (ndc.x / aspectRatio + 1.0f) * m_width * 0.5f - 0.5f;
    float screenY = (1.0f - ndc.y) * m_height * 0.5f - 0.5f;

    return glm::ivec2(static_cast<uint32_t>(screenX), static_cast<uint32_t>(screenY));
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