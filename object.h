#pragma once

#include "transform.h"
class Texture;

class Object {
public:
    Object(std::shared_ptr<MeshData> pMesh, std::shared_ptr<Texture> pTexture);
    ~Object();

public:
    Transform m_transform;

public: // mesh
    std::shared_ptr<MeshData> m_mesh;
    std::shared_ptr<Texture> m_texture;
};
