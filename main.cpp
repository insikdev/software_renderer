#include "pch.h"
#include "renderer.h"

int main()
{
    const int width = 640;
    const int height = 480;
    std::make_unique<Renderer>("Software Renderer", width, height)->Run();

    return 0;
}
