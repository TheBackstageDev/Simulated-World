#pragma once

#include <raylib.h>
#include <raymath.h>

namespace System_Utils
{
    extern Camera2D cam;
    
    bool isPositionInsideCamera(const Camera2D &camera, Vector2 pos);
    Vector2 getRealCellMousePosition(const Camera2D &camera);
} // namespace System
