#include "../src/Headers/System/Map/WorldMap.hpp"

namespace World
{
    WorldMap::WorldMap(float CellSize)
    {
        Map.resize(CellSize); // for the height

        for (auto &row : Map)
        {
            row.resize(CellSize * 2); // for the width
        }
    }

    WorldMap::~WorldMap() {}
} // namespace World
