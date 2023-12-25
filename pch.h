#pragma once

// library
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

// std
#include <array>
#include <vector>
#include <memory>
#include <string>
#include <iostream>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};