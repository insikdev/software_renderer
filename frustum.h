#pragma once

#include "plane.h"

class Frustum {
public:
    Frustum(const std::array<Plane, 6>& planes)
        : m_planes { planes }
    {
    }
    ~Frustum() = default;

public:
    bool IsCull()
    {
        for (const auto& plane : m_planes) {
        }
        return true;
    }

private:
    const std::array<Plane, 6> m_planes;
};
