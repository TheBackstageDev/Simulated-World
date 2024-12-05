#include "../src/Headers/System/Map/WorldMap.hpp"

#include <cmath>

namespace World
{
    WorldMap::WorldMap(const int cellSize) : cellSize(cellSize)
    {
        Map.resize(cellSize * PLANET_MAP_HEIGHT * PLANET_CELL_MULTIPLIER); // for the height

        for (auto &row : Map)
        {
            row.resize(cellSize * PLANET_MAP_RATIO * PLANET_MAP_HEIGHT * PLANET_CELL_MULTIPLIER); // for the width
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
                auto& CurrentCell = Map[y][x];
                float CurrentElevation = CurrentCell.getElevation();

                unsigned char colorValue = static_cast<unsigned char>(CurrentElevation * 255.0f);
                Color cellColor = {colorValue, colorValue, colorValue, 255};
                
                // For fitting the window size
                int posX = static_cast<int>((x * cellSize - camera.target.x) * scale + camera.offset.x);
                int posY = static_cast<int>((y * cellSize - camera.target.y) * scale + camera.offset.y);
                int width = static_cast<int>(1 + cellSize * scale);
                int height = static_cast<int>(1 + cellSize * scale);

                DrawRectangle(posX, posY, width, height, cellColor);
            }
        }
    }
} // namespace World
