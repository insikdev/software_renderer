#include "pch.h"
#include "renderer.h"
#include "window.h"
#include "geometry_helper.h"
#include "object.h"
#include "simple_pipeline.h"
#include "resource_manager.h"

Renderer::Renderer(std::string title, int width, int height)
{
    p_window = new Window { title, width, height };

    m_frontBuffer = SDL_GetWindowSurface(p_window->GetWindow());
    m_backBuffer = SDL_CreateRGBSurfaceWithFormat(0, m_frontBuffer->w, m_frontBuffer->h, m_frontBuffer->format->BitsPerPixel, m_frontBuffer->format->format);
    m_fragments = new Fragment[m_frontBuffer->w * m_frontBuffer->h];

    p_pipeline = new SimplePipeline {};
    p_pipeline->SetFrameBuffer(m_backBuffer);
    p_pipeline->SetFragmentBuffer(m_fragments);
    p_pipeline->SetViewport(m_backBuffer->w, m_backBuffer->h);

    // std::shared_ptr<MeshData> triangle = std::make_shared<MeshData>(GeometryHelper::CreateTriangle());
    // std::shared_ptr<MeshData> rect = std::make_shared<MeshData>(GeometryHelper::CreateRectangle());
    std::shared_ptr<MeshData> cube = std::make_shared<MeshData>(GeometryHelper::CreateCube());

    m_objects.push_back(std::make_unique<Object>(cube, ResourceManager::GetTexture("assets/wall.jpg")));
}

Renderer::~Renderer()
{
    ResourceManager::Clean();
    SDL_FreeSurface(m_backBuffer);
    delete m_fragments;
    delete p_pipeline;
    delete p_window;
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

    const uint8_t* keyboardState = SDL_GetKeyboardState(NULL);

    if (keyboardState[SDL_SCANCODE_ESCAPE]) {
        m_isRunning = false;
    }

    if (keyboardState[SDL_SCANCODE_1]) {
        p_pipeline->IsWireFrameMode = false;
    }
    if (keyboardState[SDL_SCANCODE_2]) {
        p_pipeline->IsWireFrameMode = true;
    }
    if (keyboardState[SDL_SCANCODE_3]) {
        p_pipeline->IsCullBackFace = false;
    }
    if (keyboardState[SDL_SCANCODE_4]) {
        p_pipeline->IsCullBackFace = true;
    }
}

void Renderer::Update()
{
    static uint32_t frameCount = 0;
    static uint64_t sec = 0;
    static uint64_t prev = 0;

    frameCount++;
    uint64_t current = SDL_GetTicks64(); // ms
    uint64_t dt = current - prev;
    sec += dt;

    for (auto& obj : m_objects) {
        obj->Update(dt);
    }

    if (sec >= 1000) {
        char title[16];
        snprintf(title, sizeof(title), "FPS: %d", frameCount);
        SDL_SetWindowTitle(p_window->GetWindow(), title);

        frameCount = 0;
        sec = 0;
    }
    prev = current;
}

void Renderer::Render(void)
{
    // clear buffer
    SDL_FillRect(m_backBuffer, NULL, SDL_MapRGBA(m_backBuffer->format, 0, 0, 0, 255));
    memset(m_fragments, 0, m_backBuffer->w * m_backBuffer->h * sizeof(Fragment));

    // set uniform data
    p_pipeline->SetViewMatrix(glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    p_pipeline->SetProjMatrix(glm::perspective(glm::radians(70.0f), p_window->GetAspectRatio(), 1.0f, 10.0f));

    // draw
    for (const auto& obj : m_objects) {
        p_pipeline->Draw(obj.get());
    }

    // present
    SDL_BlitSurface(m_backBuffer, NULL, m_frontBuffer, NULL);
    SDL_UpdateWindowSurface(p_window->GetWindow());
}
