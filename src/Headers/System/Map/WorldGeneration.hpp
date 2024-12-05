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

        void GenerateWorld(int seed, int res, int freq);
        bool doesMapExist() { return map != nullptr; }

        void renderMap() { map->drawMap(); }
    };
} // namespace MapGen
