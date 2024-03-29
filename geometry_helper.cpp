#include "pch.h"
#include "geometry_helper.h"

MeshData GeometryHelper::CreateTriangle(void)
{
    MeshData data;

    data.vertices = {
        { { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        { { -1.0f, -1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        { { 1.0f, -1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
    };

    data.indices = { 0, 1, 2 };

    return data;
}

MeshData GeometryHelper::CreateRectangle(void)
{
    MeshData data;

    data.vertices = {
        { { -1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        { { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
        { { 1.0f, -1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
        { { -1.0f, -1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
    };

    data.indices = { 0, 3, 2, 0, 2, 1 };

    return data;
}

MeshData GeometryHelper::CreateCube(void)
{
    MeshData data;

    data.vertices = {
        // Front face
        { { -1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        { { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
        { { 1.0f, -1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
        { { -1.0f, -1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },

        // Back face
        { { 1.0f, 1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
        { { -1.0f, 1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f } },
        { { -1.0f, -1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } },
        { { 1.0f, -1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } },

        // Top face
        { { -1.0f, 1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
        { { 1.0f, 1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
        { { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
        { { -1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },

        // Bottom face
        { { -1.0f, -1.0f, 1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
        { { 1.0f, -1.0f, 1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } },
        { { 1.0f, -1.0f, -1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } },
        { { -1.0f, -1.0f, -1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } },

        // Right face
        { { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
        { { 1.0f, 1.0f, -1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
        { { 1.0f, -1.0f, -1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
        { { 1.0f, -1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },

        // Left face
        { { -1.0f, 1.0f, -1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
        { { -1.0f, 1.0f, 1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
        { { -1.0f, -1.0f, 1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
        { { -1.0f, -1.0f, -1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } }
    };

    data.indices = {
        0, 2, 3, 0, 1, 2, // Front face
        4, 6, 7, 4, 5, 6, // Back face
        8, 10, 11, 8, 9, 10, // Top face
        12, 14, 15, 12, 13, 14, // Bottom face
        16, 18, 19, 16, 17, 18, // Right face
        20, 22, 23, 20, 21, 22 // Left face
    };

    return data;
}
