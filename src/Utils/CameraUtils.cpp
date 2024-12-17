#include "../src/Headers/Utils/CameraUtils.hpp"
#include "../src/Headers/Utils/WindowUtils.hpp"
#include "../src/Headers/System/Map/WorldGeneration.hpp"

namespace System_Utils
{
    Camera2D cam = {0};

    bool System_Utils::isPositionInsideCamera(const Camera2D &camera, Vector2 pos)
    {
        Vector2 screenPos = GetWorldToScreen2D(pos, camera);
        
        bool isOnScreenSpace = (screenPos.x >= 0 && screenPos.x <= GetScreenWidth() &&
                                screenPos.y >= 0 && screenPos.y <= GetScreenHeight());

        return isOnScreenSpace;
    }

    Vector2 System_Utils::getRealCellMousePosition(const Camera2D &camera)
    {
        auto *map = World::WorldGenerator::getMap();

        float cellSize = map->getCellSize();
        float scale = System_Utils::getCurrentWindowScaleOffset({map->getWidth() * cellSize, map->getHeight() * cellSize}) * camera.zoom;

        Vector2 mousePos = GetMousePosition();
        Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);

        worldPos = (worldPos - camera.offset) / scale + camera.target;

        return worldPos;
    }

} // namespace System_Utils
