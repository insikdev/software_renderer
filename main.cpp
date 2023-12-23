#include "renderer.h"

int main()
{
    Renderer* r = new Renderer { "Software Renderer", 640, 480 };
    r->Run();
    delete r;
    return 0;
}
