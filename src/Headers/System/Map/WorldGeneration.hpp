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
        static Planet* planet;

        static void GenerateRivers();
        static void ApplyErosion(int iterations, float erosionStrength); //Future
    public:
        WorldGenerator();
        ~WorldGenerator();

        static void GenerateWorld(int seed, float res, float freq, float CellSize);
        static void GeneratePlanet(float axialTilt, float orbitalEccentricity, float distanceFromStar, float rotationPeriod)
        {
            planet = new Planet(axialTilt, orbitalEccentricity, distanceFromStar, rotationPeriod);
        }
        void renderMap( const Camera2D &camera ) { map->drawMap(camera); }
        bool doesMapExist() { return map != nullptr; }
        
        static Planet *getPlanet() { return planet; }
        static WorldMap* getMap() { return map; }
        static GridCell& getGridCellAtPos(Vector2& worldPos);

        static Image getMapImage() { return map->getMapImage(map->getMapData(), map->getWidth(), map->getHeight(), 1); }
    };
} // namespace MapGen
