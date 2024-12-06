#include "../src/Headers/System/Map/WorldMap.hpp"
#include "../src/Headers/Utils/CameraUtils.hpp"

namespace World
{
    WorldMap::WorldMap(const float cellSize) : cellSize(cellSize)
    {
        Map.resize(getHeight()); // for the height

        for (auto &row : Map)
        {
            row.resize(getWidth()); // for the width
        }
    }

    WorldMap::~WorldMap() {}

    void WorldMap::drawMap(const Camera2D &camera)
    {
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        float scaleX = static_cast<float>(screenWidth) / (getWidth() * cellSize);
        float scaleY = static_cast<float>(screenHeight) / (getHeight() * cellSize);
        float scale = std::min(scaleX, scaleY) * camera.zoom;

        for (int x = 0; x < getWidth(); ++x)
        {
            for (int y = 0; y < getHeight(); ++y)
            {
                float posX = x * cellSize;
                float posY = y * cellSize;

                if (System::isPositionInsideCamera(camera, {posX, posY}))
                {
                    auto &CurrentCell = Map[y][x];
                    float CurrentElevation = CurrentCell.getElevation();

                    unsigned char colorValue = static_cast<unsigned char>(CurrentElevation * 255.0f);
                    Color cellColor = {colorValue, colorValue, colorValue, 255};

                    // For fitting the window size
                    int drawPosX = static_cast<int>((posX - camera.target.x) * scale + camera.offset.x);
                    int darwPosY = static_cast<int>((posY - camera.target.y) * scale + camera.offset.y);
                    int width = static_cast<int>(1 + cellSize * scale);
                    int height = static_cast<int>(1 + cellSize * scale);

                    DrawRectangle(drawPosX, darwPosY, width, height, cellColor);
                }
            }
        }
    }
} // namespace World
