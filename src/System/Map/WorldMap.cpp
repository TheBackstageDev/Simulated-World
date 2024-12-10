#include "../src/Headers/System/Map/WorldMap.hpp"
#include "../src/Headers/Utils/CameraUtils.hpp"
#include "../src/Headers/Utils/WindowUtils.hpp"

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
        float scale = System_Utils::getCurrentWindowScaleOffset({getWidth() * cellSize, getHeight() * cellSize}) * camera.zoom;

        for (int x = 0; x < getWidth(); ++x)
        {
            for (int y = 0; y < getHeight(); ++y)
            {
                float posX = x * cellSize;
                float posY = y * cellSize;

                if (System_Utils::isPositionInsideCamera(camera, {posX, posY}))
                {
                    auto &CurrentCell = Map[y][x];
                    float CurrentElevation = CurrentCell.getElevation();

                    Color cellColor = CurrentCell.getColor();

                    // For fitting the window size
                    int drawPosX = static_cast<int>((posX - camera.target.x) * scale + camera.offset.x);
                    int drawPosY = static_cast<int>((posY - camera.target.y) * scale + camera.offset.y);
                    int width = static_cast<int>(1 + cellSize * scale);
                    int height = static_cast<int>(1 + cellSize * scale);

                    DrawRectangle(drawPosX, drawPosY, width, height, cellColor);
                }
            }
        }
    }

    Image WorldMap::getMapImage(std::vector<std::vector<GridCell>> &map, int mapWidth, int mapHeight, int cellSize)
    {
        Image mapImage = GenImageColor(mapWidth * cellSize, mapHeight * cellSize, RAYWHITE);

        ImageDrawRectangle(&mapImage, 0, 0, mapWidth * cellSize, mapHeight * cellSize, WHITE);

        for (int y = 0; y < mapHeight; ++y)
        {
            for (int x = 0; x < mapWidth; ++x)
            {
                auto &currentCell = map[y][x];
                float currentElevation = currentCell.getElevation();

                unsigned char colorValue = static_cast<unsigned char>(currentElevation * 255.0f);
                Color cellColor = {colorValue, colorValue, colorValue, 255};

                ImageDrawRectangle(&mapImage, x * cellSize, y * cellSize, cellSize, cellSize, cellColor);
            }
        }

        return mapImage;
    }

} // namespace World
