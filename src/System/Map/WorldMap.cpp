#include "../src/Headers/System/Map/WorldMap.hpp"

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

    void WorldMap::drawMap()
    {
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        float scaleX = static_cast<float>(screenWidth) / (getWidth() * cellSize);
        float scaleY = static_cast<float>(screenHeight) / (getHeight() * cellSize);
        float scale = std::min(scaleX, scaleY);

        for (int x = 0; x < getWidth(); ++x)
        {
            for (int y = 0; y < getHeight(); ++y)
            {
                auto& CurrentCell = Map[y][x];
                float CurrentElevation = CurrentCell.getElevation();

                unsigned char colorValue = static_cast<unsigned char>(CurrentElevation * 255.0f);
                Color cellColor = {colorValue, colorValue, colorValue, 255};
                
                // For fitting the window size
                int posX = static_cast<int>(x * cellSize * scale);
                int posY = static_cast<int>(y * cellSize * scale);
                int width = static_cast<int>((x + 1) * cellSize * scale) - posX;
                int height = static_cast<int>((y + 1) * cellSize * scale) - posY;

                DrawRectangle(posX, posY, width, height, cellColor);
            }
        }
    }
} // namespace World
