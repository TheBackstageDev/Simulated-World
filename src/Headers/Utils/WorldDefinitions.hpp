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
    #define MAX_RIVER_LENGTH 25 * (1 / Cellsize) // in KM
    #define MAX_RIVER_WIDTH 2

    #define MOUNTAIN_PEAKS_MIN_DISTANCE 30 * (1 / Cellsize) // in KM
    #define MOUNTAIN_PEAK_MIN_HEIGHT 1.0

    //Planet Defines
    #define SOLAR_CONSTANT 1361.0f // Solar constant in W/m²
    #define UPDATE_RATE 30 // In Days

    //Simulation Defines
    extern drawMode currentDrawMode;
    extern float SimulationStep;

    #define GRAVITATIONAL_CONSTANT 6.67408e-11
    #define STAR_MASS 1.989e30 //Temporary

    //WorldMap Functions
    std::string biomeToString(Biome biome);
    bool isPeakDistantFromOthers(std::priority_queue<GridCell, std::vector<GridCell>, CompareGridCell> mountainPeaks, Vector2 peak);

} // namespace System_Utils
