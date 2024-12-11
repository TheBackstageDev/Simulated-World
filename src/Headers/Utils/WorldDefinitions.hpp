#pragma once

#include <string>
#include "../src/Headers/System/Map/WorldGeneration.hpp"

namespace World
{
    //Height Defines
    #define SEA_LEVEL 0.05f

    //WorldMap Defines
    static float Cellsize{1.0f}; // in KM
    #define MAX_RIVERS 3
    #define MAX_RIVER_LENGHT 100 // in cells

    //Simulation Defines
    extern drawMode currentDrawMode;

    std::string biomeToString(Biome biome);

} // namespace System_Utils
