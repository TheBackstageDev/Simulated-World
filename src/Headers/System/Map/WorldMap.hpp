#pragma once

#include "WorldCell.hpp"
#include <vector>
#include <raylib.h>

#define PLANET_MAP_RATIO 2 // Ratio beetwen the Height and Width of the Map Rectangle
#define TOTAL_MAP_HEIGHT 256    // Height in Grids, Based on the Cartesian
#define TOTAL_MAP_WIDTH (TOTAL_MAP_HEIGHT * PLANET_MAP_RATIO) 

namespace World
{
    class WorldMap
    {
    private:
        const float cellSize;
        std::vector<std::vector<GridCell>> Map;
    public:
        WorldMap(const float cellSize);
        ~WorldMap();

        GridCell &getCell(int x, int y) { return Map[y][x]; };
        void setCell(int x, int y, const GridCell cell) { Map[y][x] = cell; }

        int getHeight() { return TOTAL_MAP_HEIGHT / cellSize; }
        int getWidth() { return TOTAL_MAP_WIDTH / cellSize; }

        void drawMap(const Camera2D &camera);
        float getCellSize() { return cellSize; } 
        std::vector<std::vector<GridCell>> getMapData() { return Map; }

        static Image getMapImage(std::vector<std::vector<GridCell>> &map, int mapWidth, int mapHeight, int cellSize); 
    };
} // namespace World
