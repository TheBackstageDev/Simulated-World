#pragma once

#include "WorldCell.hpp"
#include <vector>

#define PLANET_RATIO 2 // Ratio beetwen the Height and Width of the Map Rectangle

namespace World
{
    class WorldMap
    {
    private:
        std::vector<std::vector<GridCell>> Map;
    public:
        WorldMap(float CellSize); 
        ~WorldMap();

        GridCell &getCell(int x, int y) { return Map[y][x]; };
        void setCell(int x, int y, const GridCell cell) { Map[y][x] = cell; }
    };
} // namespace World
