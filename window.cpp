#include "window.h"
#include <iostream>

Window::Window(std::string title, int width, int height)
    : m_title { title }
    , m_width { width }
    , m_height { height }
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Failed to initialize SDL: " << SDL_GetError();
        exit(EXIT_FAILURE);
    }

    m_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, 0);

    if (!m_window) {
        std::cout << "Failed to create window: " << SDL_GetError();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

Window::~Window()
{
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
