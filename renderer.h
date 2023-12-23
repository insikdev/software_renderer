#pragma once

#include <memory>
#include <string>
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

private:
    std::unique_ptr<const Window> p_window;
    bool m_isRunning { true };

private: // fps
    uint64_t m_prevFrameTime {};
    uint64_t m_frameCount {};
};
