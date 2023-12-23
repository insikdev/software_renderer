#pragma once

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <string>

class Window {
public:
    Window(std::string title, int width, int height);
    ~Window();

public: // getter
    SDL_Window* GetWindow(void) const { return m_window; }
    int GetWidth(void) const { return m_width; }
    int GetHeight(void) const { return m_height; }

private:
    SDL_Window* m_window {};
    std::string m_title;
    int m_width;
    int m_height;
};
