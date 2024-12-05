#pragma once

#include "WorldCell.hpp"
#include <vector>
#include <raylib.h>

#define PLANET_MAP_RATIO 2 // Ratio beetwen the Height and Width of the Map Rectangle
#define PLANET_MAP_HEIGHT 16 // Height in Grids, Based on the Cartesian
#define PLANET_CELL_MULTIPLIER 2 // Multiplier to increase ammount of cells

namespace World
{
    class WorldMap
    {
    private:
        const int cellSize;
        std::vector<std::vector<GridCell>> Map;
    public:
        WorldMap(const int cellSize); 
        ~WorldMap();

        GridCell &getCell(int x, int y) { return Map[y][x]; };
        void setCell(int x, int y, const GridCell cell) { Map[y][x] = cell; }

        int getHeight() { return cellSize * PLANET_MAP_HEIGHT * PLANET_CELL_MULTIPLIER; }
        int getWidth() { return cellSize * PLANET_MAP_HEIGHT * PLANET_MAP_RATIO * PLANET_CELL_MULTIPLIER; }

        void drawMap(const Camera2D &camera);
    };
} // namespace World
