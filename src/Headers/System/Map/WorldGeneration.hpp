#pragma once

#include "WorldMap.hpp"
#include "../src/Headers/Utils/WorldDefinitions.hpp"
#include "../PerlinNoise.hpp"

namespace World
{
    class WorldGenerator
    {
    private:
        static siv::PerlinNoise perlinNoise;
        static WorldMap* map;

        static void GenerateRivers();
        static void ApplyErosion(int iterations, float erosionStrength); //Future
    public:
        WorldGenerator();
        ~WorldGenerator();

        static void GenerateWorld(int seed, float res, float freq, float CellSize);
        void renderMap( const Camera2D &camera ) { map->drawMap(camera); }
        bool doesMapExist() { return map != nullptr; }
        static WorldMap* getMap() { return map; }
        static GridCell& getGridCellAtPos(Vector2& worldPos);

        static Image getMapImage() { return map->getMapImage(map->getMapData(), map->getWidth(), map->getHeight(), 1); }
    };
} // namespace MapGen
