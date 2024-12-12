#include "../src/Headers/System/Map/WorldMap.hpp"
#include "../src/Headers/Utils/CameraUtils.hpp"
#include "../src/Headers/Utils/WindowUtils.hpp"
#include "../src/Headers/Utils/WorldDefinitions.hpp"

#include <iostream>

namespace World
{
    std::priority_queue<GridCell, std::vector<GridCell>, CompareGridCell> WorldMap::mountainPeaks;

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

        Vector2 topLeft = GetScreenToWorld2D({0, 0}, camera);
        Vector2 bottomRight = GetScreenToWorld2D({(float)GetScreenWidth(), (float)GetScreenHeight()}, camera);

        topLeft = (topLeft - camera.offset) / scale + camera.target;
        bottomRight = (bottomRight - camera.offset) / scale + camera.target;

        int startX = static_cast<int>(std::max(0.0f, floor((topLeft.x) / cellSize)));
        int endX = static_cast<int>(std::min((float)getWidth(), ceil((bottomRight.x) / cellSize)));
        int startY = static_cast<int>(std::max(0.0f, floor((topLeft.y) / cellSize)));
        int endY = static_cast<int>(std::min((float)getHeight(), ceil((bottomRight.y) / cellSize)));

        int ammount = 0;
        for (int x = startX; x < endX; ++x)
        {
            for (int y = startY; y < endY; ++y)
            {
                float posX = x * cellSize;
                float posY = y * cellSize;

                int drawPosX = static_cast<int>((posX - camera.target.x) * scale + camera.offset.x);
                int drawPosY = static_cast<int>((posY - camera.target.y) * scale + camera.offset.y);

                auto &CurrentCell = Map[y][x];
                float CurrentElevation = CurrentCell.getElevation();

                Color cellColor;

                switch (World::currentDrawMode)
                {
                case drawMode::Terrain:
                    cellColor = CurrentCell.getColor();
                    break;
                case drawMode::Elevation:
                {
                    float clampedElevation = std::max(-1.0f, std::min(CurrentElevation, 1.0f));
                    float normalizedElevation = (clampedElevation + 1.0f) / 2.0f;

                    unsigned char elevationRgb = static_cast<unsigned char>(clampedElevation * 255);
                    cellColor = {elevationRgb, elevationRgb, elevationRgb, 255};
                    break;
                }
                case drawMode::Biomes:
                    cellColor = CurrentCell.getBiomeColor();
                    break;
                case drawMode::Humidity:
                    cellColor = CurrentCell.getHumidityColor();
                    break;
                case drawMode::Temperature:
                    cellColor = CurrentCell.getTemperatureColor();
                    break;
                default:
                    cellColor = {255, 255, 255, 255};
                    break;
                }

                // For fitting the window size
                int width = static_cast<int>(1 + cellSize * scale);
                int height = static_cast<int>(1 + cellSize * scale);

                DrawRectangle(drawPosX, drawPosY, width, height, cellColor);

                ammount++;
            }
        }

        DrawText(TextFormat("Cells Drawn: %i", ammount), GetScreenWidth() / 2, GetScreenHeight() / 2, 10.f, WHITE);
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
                ImageDrawRectangle(&mapImage, x * cellSize, y * cellSize, cellSize, cellSize, currentCell.getColor());
            }
        }

        return mapImage;
    }

} // namespace World
