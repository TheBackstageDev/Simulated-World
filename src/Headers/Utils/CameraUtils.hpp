#pragma once

#include <raylib.h>
#include <raymath.h>

namespace System_Utils
{
    extern Camera2D cam;
    
    Camera2D& getCam();
    bool isPositionInsideCamera(const Camera2D &camera, Vector2 pos);
} // namespace System
