#pragma once

#include <string>
#include "../src/Headers/System/Map/WorldGeneration.hpp"

namespace World
{
    //Height Defines
    #define SEA_LEVEL 0.05f

    //WorldMap Defines
    static float Cellsize{1.0f}; // in KM

    std::string biomeToString(Biome biome);

} // namespace System_Utils
