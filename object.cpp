#include "pch.h"
#include "object.h"

Object::Object(std::shared_ptr<MeshData> pMesh, std::shared_ptr<Texture> pTexture)
    : m_mesh { pMesh }
    , m_texture { pTexture }
{
}

Object::~Object()
{
}
