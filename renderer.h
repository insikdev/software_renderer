#pragma once

class Window;

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

private:
    void RenderTriangle(const std::vector<Vertex>& vertices);

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
};
