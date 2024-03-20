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
    Window* p_window;
    Pipeline* p_pipeline;

private:
    bool m_isRunning { true };
    SDL_Surface* m_frontBuffer;
    SDL_Surface* m_backBuffer;
    Fragment* m_fragments;

private: // scene
    std::vector<std::unique_ptr<Object>> m_objects;
};
