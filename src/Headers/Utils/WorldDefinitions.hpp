#pragma once

#include <string>
#include "../src/Headers/System/Map/WorldGeneration.hpp"

namespace World
{
    //Height Defines
    #define SEA_LEVEL 0.05f

    //WorldMap Defines
    static float Cellsize{1.0f}; // in KM
    #define MAX_RIVERS 10
    #define MAX_RIVER_LENGTH 50 * (1 / Cellsize) // in KM

    #define MOUNTAIN_PEAKS_MIN_DISTANCE 30 * (1 / Cellsize) // in KM
    #define MOUNTAIN_PEAK_MIN_HEIGHT 1.0 

    //Simulation Defines
    extern drawMode currentDrawMode;

    //WorldMap Functions
    std::string biomeToString(Biome biome);
    bool isPeakDistantFromOthers(std::priority_queue<GridCell, std::vector<GridCell>, CompareGridCell> mountainPeaks, Vector2 peak);

} // namespace System_Utils
