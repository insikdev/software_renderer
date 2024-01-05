#pragma once

class Texture;
class Object;

class Pipeline {
public:
    struct UniformData {
        glm::mat4 world = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);
        std::shared_ptr<Texture> texture;
    };

public:
    Pipeline() = default;
    virtual ~Pipeline() = default;

public:
    void Draw(const Object* obj);

public: // setter
    void SetFrameBuffer(SDL_Surface* pSurface)
    {
        m_pixels = (uint32_t*)pSurface->pixels;
        m_format = pSurface->format;
        m_totalPixels = pSurface->w * pSurface->h;
    }
    void SetFragmentBuffer(Fragment* pFragments) { m_fragments = pFragments; }
    void SetViewport(int width, int height)
    {
        m_width = width;
        m_height = height;
    }

public: // uniform
    void SetViewMatrix(const glm::mat4& view) { m_uniform.view = view; }
    void SetProjMatrix(const glm::mat4& proj) { m_uniform.proj = proj; }

private:
    int ComputeOutCode(const glm::ivec2& input, const glm::ivec2& min, const glm::ivec2& max);
    bool LineClip(glm::ivec2& p0, glm::ivec2& p1, const glm::ivec2& min, const glm::ivec2& max);
    void DrawLine(glm::ivec2 p0, glm::ivec2 p1);
    float EdgeFunction(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2);
    bool FrustumCulling(const glm::mat4& mvp, const AABB& boundingVolume);
    void Rasterizer(const std::array<Vertex, 3>& vertices);
    glm::ivec2 NDCToScreen(const glm::vec3& ndc);

protected:
    virtual void VertexShader(std::vector<Vertex>& vertices) = 0;
    virtual void FragmentShader() = 0;

protected:
    uint32_t* m_pixels {};
    SDL_PixelFormat* m_format {};
    uint64_t m_totalPixels {};
    Fragment* m_fragments {};
    UniformData m_uniform {};
    int m_width { 640 };
    int m_height { 480 };

public: // pipeline state
    bool IsWireFrameMode { false };
    bool IsCullBackFace { false };
};
