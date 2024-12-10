#include "../src/Headers/Utils/CameraUtils.hpp"

namespace System_Utils
{
    Camera2D cam = {0};

    Camera2D &System_Utils::getCam() { return cam; }
    bool System_Utils::isPositionInsideCamera(const Camera2D &camera, Vector2 pos)
    {
        float invScale = 1.0f / camera.zoom;

        // Calculate the visible area in world coordinates
        float startX = camera.target.x - (camera.offset.x * invScale);
        float startY = camera.target.y - (camera.offset.y * invScale);
        float endX = camera.target.x + (camera.offset.x * invScale) + ((GetScreenWidth() - camera.offset.x) * invScale);
        float endY = camera.target.y + (camera.offset.y * invScale) + ((GetScreenHeight() - camera.offset.y) * invScale);

        // Check if the position is within the visible area
        bool inViewX = pos.x >= startX && pos.x <= endX;
        bool inViewY = pos.y >= startY && pos.y <= endY;

        return inViewX && inViewY;
    }
} // namespace System_Utils
