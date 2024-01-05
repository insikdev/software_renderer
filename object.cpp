#include "pch.h"
#include "object.h"

Object::Object(std::shared_ptr<MeshData> pMesh, std::shared_ptr<Texture> pTexture)
    : m_mesh { pMesh }
    , m_texture { pTexture }
{
    CreateBoundingVolume(pMesh->vertices);
}

void Object::Update(uint64_t dt)
{
    const uint8_t* keyboardState = SDL_GetKeyboardState(NULL);

    if (keyboardState[SDL_SCANCODE_W]) {
        m_transform.MoveY(dt);
    }
    if (keyboardState[SDL_SCANCODE_S]) {
        m_transform.MoveY(dt * -1);
    }
    if (keyboardState[SDL_SCANCODE_A]) {
        m_transform.MoveX(dt * -1);
    }
    if (keyboardState[SDL_SCANCODE_D]) {
        m_transform.MoveX(dt);
    }
    if (keyboardState[SDL_SCANCODE_Q]) {
        m_transform.MoveZ(dt * -1);
    }
    if (keyboardState[SDL_SCANCODE_E]) {
        m_transform.MoveZ(dt);
    }
    if (keyboardState[SDL_SCANCODE_R]) {
        m_transform.m_position = glm::vec3(0.0f);
    }
}

void Object::CreateBoundingVolume(const std::vector<Vertex> vertices)
{
    glm::vec3& min = m_boundingVolume.min;
    glm::vec3& max = m_boundingVolume.max;

    for (const auto& v : vertices) {
        min.x = glm::min(min.x, v.position.x);
        min.y = glm::min(min.y, v.position.y);
        min.z = glm::min(min.z, v.position.z);

        max.x = glm::max(max.x, v.position.x);
        max.y = glm::max(max.y, v.position.y);
        max.z = glm::max(max.z, v.position.z);
    }
}
