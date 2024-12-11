#include "../src/Headers/System/Map/WorldGeneration.hpp"

#include <cassert>
#include <cmath>

namespace World
{
    WorldMap *World::WorldGenerator::map = nullptr;
    siv::PerlinNoise World::WorldGenerator::perlinNoise{0};

    WorldGenerator::WorldGenerator()
    {
    }

    WorldGenerator::~WorldGenerator() {}

    void WorldGenerator::GenerateRivers()
    {
        int map_height = map->getHeight();
        int map_width = map->getWidth();
        int riverCount = 0;

        const float startElevationThreshold = 0.8f; // Height at which rivers will start forming
        const float maxElevationThreshold = 1.2f; // Max height for river formation

        for (int y = 0; y < map_height; ++y)
        {
            for (int x = 0; x < map_width; ++x)
            {
                if (riverCount >= MAX_RIVERS)
                    return;
            }   
        }
    }

    void WorldGenerator::GenerateWorld(int seed, float res, float freq, float CellSize)
    {
        delete map;
        perlinNoise = siv::PerlinNoise(seed);
        map = new WorldMap(CellSize);

        int map_height = map->getHeight();
        int map_width = map->getWidth();

        //Amp means frequency of variation
        float hillFreq = freq * 0.5f;
        float mountainFreq = freq * 0.5f;

        //Main loop, for Map Creation
        for (int y = 0; y < map_height; ++y)
        {
            for (int x = 0; x < map_width; ++x)
            {
                float nx = static_cast<float>(x) / map_width * res;
                float ny = static_cast<float>(y) / map_height * res;

                float elevation = perlinNoise.normalizedOctave2D(nx, ny, 10) * 2;

                //Hills and Mountains
                float hillElevation = perlinNoise.normalizedOctave2D(nx * hillFreq, ny * hillFreq, 15);
                float mountainElevation = perlinNoise.normalizedOctave2D(nx * mountainFreq, ny * mountainFreq, 10) / 2;
                elevation += hillElevation + mountainElevation;

                float latitude = static_cast<float>(y) / map_height;
                float latitudeFactor = 1.0f - abs(2.0f * latitude - 1.0f);

                // Temperature and Humidity
                float elevationFactor = 1.0f;
  
                if (elevation > SEA_LEVEL)
                {
                    elevationFactor = 1.0f - (elevation * elevation);
                }

                float temperature = (latitudeFactor * 0.75f + elevationFactor * 0.2f);

                float humidityNoise = perlinNoise.normalizedOctave2D(nx + 10.0f, ny + 10.0f, 10);
                float humidity = (latitudeFactor * 0.4f + elevationFactor * 0.3f + humidityNoise * 0.4f);

                humidity = std::clamp(humidity, 0.0f, 1.0f);

                //Is Ocean so its naturally Humid
                if (elevation < SEA_LEVEL && temperature > 0.3f)
                    humidity += 0.05f;

                GridCell cell(std::string(std::to_string(x) + "-" + std::to_string(y)), elevation, temperature, humidity);
                map->setCell(x, y, cell);
            }
        }

        //Secondary Loops, for Details
        GenerateRivers();
    }

    GridCell &WorldGenerator::getGridCellAtPos(Vector2 &worldPos)
    {
        assert(map != nullptr && "Map does not exist!");

        int cellX = static_cast<int>(worldPos.x / map->getCellSize());
        int cellY = static_cast<int>(worldPos.y / map->getCellSize());

        if (cellX >= 0 && cellX < map->getWidth() && cellY >= 0 && cellY < map->getHeight())
        {
            return map->getCell(cellX, cellY);
        }

        return GridCell();
    }

} // namespace World
