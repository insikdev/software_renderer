#include "pch.h"
#include "geometry_helper.h"

MeshData GeometryHelper::CreateTriangle(void)
{
    MeshData data;

    data.vertices = {
        { { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
        { { -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
        { { 1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
    };

    data.indices = { 0, 1, 2 };

    return data;
}
