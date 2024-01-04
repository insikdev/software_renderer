#pragma once

class Window;
class Texture;
class Object;
class Pipeline;

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
    bool m_isWireFrameMode { false };

private:
    Pipeline* p_pipeline;
    SDL_Surface* m_frontBuffer;
    SDL_Surface* m_backBuffer;
    Fragment* m_fragments;

private: // scene
    std::vector<Object*> m_objects;
};
