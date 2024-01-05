#pragma once

#include "transform.h"
class Texture;

class Object {
public:
    Object(std::shared_ptr<MeshData> pMesh, std::shared_ptr<Texture> pTexture);
    ~Object() = default;

public:
    void Update(uint64_t dt);

private:
    void CreateBoundingVolume(const std::vector<Vertex> vertices);

public:
    Transform m_transform;

public: // mesh
    std::shared_ptr<MeshData> m_mesh;
    std::shared_ptr<Texture> m_texture;
    AABB m_boundingVolume { glm::vec3(0.0f), glm::vec3(0.0f) };
};
