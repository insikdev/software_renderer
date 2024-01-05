#pragma once

// SDL
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// std
#include <array>
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>

struct Vertex {
    glm::vec4 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

struct Fragment {
    glm::vec2 uv;
    float depth;
    bool active;
};

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};