#pragma once

class Texture;

class ResourceManager {
public:
    static void Initialize(void);
    static void Clean(void);
    static std::shared_ptr<Texture> GetTexture(std::string filename);

private:
    static std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
};
