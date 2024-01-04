#pragma once

class Plane {
public:
    Plane(glm::vec4 input)
    {
        glm::vec3 normal = glm::vec3(input);
        float inverseSqrt = 1 / glm::length(normal);

        m_normal = normal * inverseSqrt;
        m_d = input.w * inverseSqrt;
    };
    ~Plane() = default;

public:
    float DistanceFromPoint(glm::vec3 point) const { return glm::dot(m_normal, point) + m_d; };

public:
    glm::vec3 m_normal;
    float m_d;
};
