#pragma once

class Transform {
public:
    Transform() = default;
    ~Transform() = default;

public:
    glm::mat4 GetWorldMatrix(void) const;

public:
    void MoveX(float dx) { m_position.x += m_moveSpeed * dx; }
    void MoveY(float dy) { m_position.y += m_moveSpeed * dy; }
    void MoveZ(float dz) { m_position.z += m_moveSpeed * dz; }

public:
    glm::vec3 m_scale { glm::vec3(1.0f) };
    glm::vec3 m_rotation { glm::vec3(0.0f) };
    glm::vec3 m_position { glm::vec3(0.0f) };

private:
    float m_moveSpeed { 0.05f };
};
