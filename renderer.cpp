#include "pch.h"
#include "renderer.h"
#include "window.h"
#include "geometry_helper.h"
#include "texture.h"

Renderer::Renderer(std::string title, int width, int height)
    : m_fragments(width * height)
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
    // m_texture = std::make_unique<Texture>("assets/awesomeface.png");
    m_texture = std::make_unique<Texture>("assets/wall.jpg");

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
    m_angle += 0.001f * dt;
}

void Renderer::Render(void)
{
    // clear buffer
    SDL_FillRect(m_backBuffer, NULL, SDL_MapRGBA(m_backBuffer->format, 0, 0, 0, 255));
    // memset(m_fragments.data(), 0, m_fragments.size() * sizeof(Fragment));

    // MeshData data = GeometryHelper::CreateRectangle();
    MeshData data = GeometryHelper::CreateCube();

    auto& vertices = data.vertices;
    auto& indices = data.indices;

    VertexShader(vertices);

    for (int i = 0; i < indices.size(); i += 3) {
        Rasterizer({ vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]] });
    }

    FragmentShader();

    Present();
}

void Renderer::VertexShader(std::vector<Vertex>& vertices)
{
    glm::mat4 world = glm::rotate(glm::mat4(1.0f), glm::radians(m_angle), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 proj = glm::perspective(glm::radians(70.0f), p_window->GetAspectRatio(), 1.0f, 50.0f);

    for (Vertex& v : vertices) {
        v.position = proj * view * world * v.position;
    }
}

void Renderer::Rasterizer(const std::array<Vertex, 3>& vertices)
{
    // perspective division
    glm::vec3 ndc0 = glm::vec3(vertices[0].position / vertices[0].position.w);
    glm::vec3 ndc1 = glm::vec3(vertices[1].position / vertices[1].position.w);
    glm::vec3 ndc2 = glm::vec3(vertices[2].position / vertices[2].position.w);

    // viewport transform
    glm::uvec2 p0 = NDCToScreen(ndc0);
    glm::uvec2 p1 = NDCToScreen(ndc1);
    glm::uvec2 p2 = NDCToScreen(ndc2);

    int minX = std::min({ p0.x, p1.x, p2.x });
    minX = std::max(0, minX);
    int maxX = std::max({ p0.x, p1.x, p2.x });
    maxX = std::min(p_window->GetWidth(), maxX);

    int minY = std::min({ p0.y, p1.y, p2.y });
    minY = std::max(0, minY);
    int maxY = std::max({ p0.y, p1.y, p2.y });
    maxY = std::min(p_window->GetHeight(), maxY);

    for (int y = minY; y < maxY; y++) {
        for (int x = minX; x < maxX; x++) {
            int index = x + y * p_window->GetWidth();
            glm::vec2 pixel = glm::vec2(static_cast<float>(x), static_cast<float>(y));

            glm::vec3 bc = CalculateBarycentricCoordinate(pixel, p0, p1, p2);

            if ((bc.x >= 0.0f) && (bc.x <= 1.0f) && (bc.y >= 0.0f) && (bc.y <= 1.0f) && (bc.z >= 0.0f) && (bc.z <= 1.0f)) {
                float depth = bc.x * ndc0.z + bc.y * ndc1.z + bc.z * ndc2.z;

                // depth test
                if (m_fragments[index].active && depth > m_fragments[index].depth) {
                    continue;
                }

                float invZ0 = 1.0f / vertices[0].position.w;
                float invZ1 = 1.0f / vertices[1].position.w;
                float invZ2 = 1.0f / vertices[2].position.w;
                float invZ = 1.0f / (invZ0 * bc.x + invZ1 * bc.y + invZ2 * bc.z);

                // interpolation using barycentric coordinate
                m_fragments[index].active = true;
                m_fragments[index].depth = depth;
                m_fragments[index].uv = (bc.x * vertices[0].uv * invZ0 + bc.y * vertices[1].uv * invZ1 + bc.z * vertices[2].uv * invZ2) * invZ;
            }
        }
    }
}

void Renderer::FragmentShader(void)
{
    Uint32* pixels = (Uint32*)m_backBuffer->pixels;

    for (uint32_t i = 0; i < m_fragments.size(); i++) {
        if (m_fragments[i].active) {
            uint8_t* color = m_texture->GetRGBA(m_fragments[i].uv);
            pixels[i] = SDL_MapRGBA(m_backBuffer->format, *color, *(color + 1), *(color + 2), 255);
            m_fragments[i].active = false;
        }
    }
}

void Renderer::Present(void)
{
    SDL_BlitSurface(m_backBuffer, NULL, m_frontBuffer, NULL);
    SDL_UpdateWindowSurface(p_window->GetWindow());
}

// viewport transform
// origin of screen space is top-left
// ndcX : [-1, 1] -> screenX : [-0.5, width - 0.5]
// ndcY : [-1, 1] -> screenY : [-0.5, height - 0.5]
glm::uvec2 Renderer::NDCToScreen(const glm::vec3& ndc)
{
    int w = p_window->GetWidth();
    int h = p_window->GetHeight();
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
