#include "../src/Headers/Utils/CameraUtils.hpp"

namespace System_Utils
{
    Camera2D cam = {0};

    Camera2D &System_Utils::getCam() { return cam; }
    bool System_Utils::isPositionInsideCamera(const Camera2D &camera, Vector2 pos)
    {
        Vector2 screenPos = GetWorldToScreen2D(pos, camera);

        bool isOnScreenSpace = (screenPos.x >= 0 && screenPos.x <= GetScreenWidth() &&
                                screenPos.y >= 0 && screenPos.y <= GetScreenHeight());

        return isOnScreenSpace;
    }

} // namespace System_Utils
