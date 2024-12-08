#pragma once

#include <raylib.h>
#include <algorithm>

namespace System_Utils
{
    static float scaleX{1};
    static float scaleY{1};
    static float windowWidth{800};
    static float windowHeight{500};

    void updateScales();
    float getCurrentWindowScaleOffset(Vector2 referenceSize);
    Vector2 getScreenCenter();
    void scaleRectangle(Rectangle &rect);
} // namespace System
