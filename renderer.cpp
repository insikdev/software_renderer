#include "renderer.h"
#include "window.h"
#include <SDL2/SDL.h>

Renderer::Renderer(std::string title, int width, int height)
{
    p_window = std::make_unique<Window>(title, width, height);
}

Renderer::~Renderer()
{
}

void Renderer::Run(void)
{
    while (m_isRunning) {
        HandleEvent();
        Update();
        Render();
    }
}

void Renderer::HandleEvent(void)
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            m_isRunning = false;
        }
    }
}

void Renderer::Update()
{
    m_frameCount++;
    uint64_t currentFrameTime = SDL_GetTicks64();
    uint64_t dt = currentFrameTime - m_prevFrameTime;

    if (dt >= 1000) {
        float fps = m_frameCount / (dt / 1000.0f);
        m_frameCount = 0;
        m_prevFrameTime = currentFrameTime;

        char title[16];
        snprintf(title, sizeof(title), "FPS: %.2f", fps);
        SDL_SetWindowTitle(p_window->GetWindow(), title);
    }
}

void Renderer::Render(void)
{
    SDL_Delay(32);
}
