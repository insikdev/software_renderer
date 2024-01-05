#include "pch.h"
#include "resource_manager.h"
#include "texture.h"

std::unordered_map<std::string, std::shared_ptr<Texture>> ResourceManager::m_textures;

void ResourceManager::Clean(void)
{
    m_textures.clear();
}

std::shared_ptr<Texture> ResourceManager::GetTexture(std::string filename)
{
    auto it = m_textures.find(filename);

    if (it != m_textures.end()) {
        return it->second;
    }

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(filename);
    m_textures[filename] = texture;
    return texture;
}
