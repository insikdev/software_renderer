#include "pch.h"
#include "renderer.h"
#include "window.h"
#include "geometry_helper.h"

Renderer::Renderer(std::string title, int width, int height)
{
    p_window = std::make_unique<Window>(title, width, height);
    m_frontBuffer = SDL_GetWindowSurface(p_window->GetWindow());
    m_backBuffer = SDL_CreateRGBSurfaceWithFormat(0, m_frontBuffer->w, m_frontBuffer->h, m_frontBuffer->format->BitsPerPixel, m_frontBuffer->format->format);
}

Renderer::~Renderer()
{
    SDL_FreeSurface(m_backBuffer);
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
    /* This assumes that color value zero is black. Use SDL_MapRGBA() for more robust surface color mapping! */
    /* height times pitch is the size of the surface's whole buffer. */
    SDL_memset(m_backBuffer->pixels, 0, m_backBuffer->h * m_backBuffer->pitch);

    Uint32* pixels = (Uint32*)m_backBuffer->pixels;

    // for (int i = 10000; i < 100000; i++) {
    //     pixels[i] = SDL_MapRGB(m_backBuffer->format, 100, 100, 100);
    // }

    // for (int i = 0; i < 3; i++) {
    //     auto p = NDCToScreen(pos[i] * 0.5f);
    //     int index = p.x + p.y * p_window->GetWidth();

    //    pixels[index] = SDL_MapRGB(m_backBuffer->format, 255, 255, 255);
    //}
    std::array<glm::vec2, 3> triangle1 = { glm::vec2(-1.0f, 1.0f), glm::vec2(-1.0f, -1.0f), glm::vec2(1.0f, -1.0f) };
    std::array<glm::vec2, 3> triangle2 = { glm::vec2(-1.0f, 1.0f), glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 1.0f) };

    // RenderTriangle(triangle1);
    // RenderTriangle(triangle2);

    MeshData data = GeometryHelper::CreateTriangle();
    RenderTriangle(data.vertices);

    Present();
}

void Renderer::Present(void)
{
    SDL_BlitSurface(m_backBuffer, NULL, m_frontBuffer, NULL);
    SDL_UpdateWindowSurface(p_window->GetWindow());
}

void Renderer::RenderTriangle(const std::vector<Vertex>& vertices)
{
    Uint32* pixels = (Uint32*)m_backBuffer->pixels;

    float scale = 0.5f;

    glm::uvec2 screenPos[] = { NDCToScreen(vertices[0].position * scale), NDCToScreen(vertices[1].position * scale), NDCToScreen(vertices[2].position * scale) };

    int minX = std::min({ screenPos[0].x, screenPos[1].x, screenPos[2].x });
    int maxX = std::max({ screenPos[0].x, screenPos[1].x, screenPos[2].x });
    int minY = std::min({ screenPos[0].y, screenPos[1].y, screenPos[2].y });
    int maxY = std::max({ screenPos[0].y, screenPos[1].y, screenPos[2].y });

    // using bounding box fps = 1400 in release
    // loop all pixel fps = 650

    // for (int y = 0; y < p_window->GetHeight(); y++) {
    //     for (int x = 0; x < p_window->GetWidth(); x++) {
    for (int y = minY; y < maxY; y++) {
        for (int x = minX; x < maxX; x++) {
            int index = x + y * p_window->GetWidth();
            glm::vec2 pixel = glm::vec2(static_cast<float>(x), static_cast<float>(y));

            auto bc = CalculateBarycentricCoordinate(pixel, screenPos[0], screenPos[1], screenPos[2]);

            if ((bc.x >= 0.0f) && (bc.x <= 1.0f) && (bc.y >= 0.0f) && (bc.y <= 1.0f) && (bc.z >= 0.0f) && (bc.z <= 1.0f)) {

                float r = bc.x * vertices[0].color.r + bc.y * vertices[1].color.r + bc.z * vertices[2].color.r;
                float g = bc.x * vertices[0].color.g + bc.y * vertices[1].color.g + bc.z * vertices[2].color.g;
                float b = bc.x * vertices[0].color.b + bc.y * vertices[1].color.b + bc.z * vertices[2].color.b;

                pixels[index] = SDL_MapRGB(m_backBuffer->format, static_cast<Uint8>(r * 255), static_cast<Uint8>(g * 255), static_cast<Uint8>(b * 255));
                // pixels[index] = SDL_MapRGB(m_backBuffer->format, 255, 255, 255);
            }
        }
    }
}

// viewport transform
// origin of screen space is top-left
// ndcX : [-1, 1] -> screenX : [-0.5, width - 0.5]
// ndcY : [-1, 1] -> screenY : [-0.5, height - 0.5]
glm::uvec2 Renderer::NDCToScreen(const glm::vec3& ndc)
{
    const int w = p_window->GetWidth();
    const int h = p_window->GetHeight();
    float aspectRatio = static_cast<float>(w) / h;

    float screenX = (ndc.x / aspectRatio + 1.0f) * w * 0.5f - 0.5f;
    float screenY = (1.0f - ndc.y) * h * 0.5f - 0.5f;

    return glm::uvec2(static_cast<uint32_t>(screenX), static_cast<uint32_t>(screenY));
}

glm::vec3 Renderer::CalculateBarycentricCoordinate(const glm::vec2& target, const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2)
{
    glm::vec2 w = target - p2;
    glm::vec2 u = p0 - p2;
    glm::vec2 v = p1 - p2;

    float wdotu = glm::dot(w, u);
    float wdotv = glm::dot(w, v);
    float udotv = glm::dot(u, v);
    float udotu = glm::dot(u, u);
    float vdotv = glm::dot(v, v);

    float denom = udotv * udotv - udotu * vdotv;

    float s = (wdotv * udotv - wdotu * vdotv) / denom;
    float t = (wdotu * udotv - wdotv * udotu) / denom;

    return glm::vec3(s, t, 1.0f - s - t);
}
