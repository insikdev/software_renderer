#include "pch.h"
#include "transform.h"

glm::mat4 Transform::GetWorldMatrix(void) const
{
    glm::mat4 world = glm::mat4(1.0f);

    world = glm::scale(world, m_scale);
    world = glm::rotate(world, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    world = glm::rotate(world, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    world = glm::rotate(world, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    world = glm::translate(world, m_position);

    return world;
}
