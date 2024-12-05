#pragma once

#include "WorldMap.hpp"
#include "../PerlinNoise.hpp"

namespace World
{
    class WorldGenerator
    {
    private:
        siv::PerlinNoise perlinNoise;
        WorldMap* map{nullptr};
    public:
        WorldGenerator();
        ~WorldGenerator();

        void GenerateWorld(int seed, int res, int freq, int CellSize);
        void renderMap( const Camera2D &camera ) { map->drawMap(camera); }
        bool doesMapExist() { return map != nullptr; }
        WorldMap* getMap() { return map; }
    };
} // namespace MapGen
