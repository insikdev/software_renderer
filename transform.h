#pragma once

class Transform {
public:
    Transform() = default;
    ~Transform() = default;

public:
    glm::mat4 GetWorldMatrix(void) const;

public:
    void MoveX(int dx) { m_position.x += m_speed * dx; }
    void MoveY(int dy) { m_position.y += m_speed * dy; }
    void MoveZ(int dz) { m_position.z += m_speed * dz; }

public: // setter
    void SetScale(const glm::vec3& scale) { m_scale = scale; }
    void SetRotation(const glm::vec3& rotation) { m_rotation = rotation; }
    void SetPosition(const glm::vec3& position) { m_position = position; }

public:
    glm::vec3 m_scale { glm::vec3(1.0f) };
    glm::vec3 m_rotation { glm::vec3(0.0f) };
    glm::vec3 m_position { glm::vec3(0.0f) };

private:
    float m_speed { 0.005f };
};
