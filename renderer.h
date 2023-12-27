#pragma once

class Window;
class Texture;

class Renderer {
public:
    Renderer(std::string title, int width, int height);
    ~Renderer();

public:
    void Run(void);

private:
    void HandleEvent(void);
    void Update(void);
    void Render(void);
    void Present(void);

private: // pipeline
    void VertexShader(std::vector<Vertex>& vertices);
    void Rasterizer(const std::array<Vertex, 3>& vertices);
    void FragmentShader(void);

private:
    void RenderTriangle(const std::vector<Vertex>& vertices);
    void RenderTriangleByIndex(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

private: // util
    glm::uvec2 NDCToScreen(const glm::vec3& ndc);
    glm::vec3 CalculateBarycentricCoordinate(const glm::vec2& target, const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2);

private:
    std::unique_ptr<const Window> p_window;
    bool m_isRunning { true };

private: // fps
    uint64_t m_prevFrameTime {};
    uint64_t m_frameCount {};

private:
    SDL_Surface* m_frontBuffer;
    SDL_Surface* m_backBuffer;
    std::unique_ptr<Texture> m_texture;
    std::vector<Fragment> m_fragments;
};
