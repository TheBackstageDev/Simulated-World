#include "../src/Headers/System/Map/WorldGeneration.hpp"

namespace World
{
    WorldMap *World::WorldGenerator::map = nullptr;
    siv::PerlinNoise World::WorldGenerator::perlinNoise{0};

    WorldGenerator::WorldGenerator()
    {
    }

    WorldGenerator::~WorldGenerator() {}


    void WorldGenerator::GenerateWorld(int seed, float res, float freq, float CellSize)
    {
        delete map;
        perlinNoise = siv::PerlinNoise(seed);
        map = new WorldMap(CellSize);

        int map_height = map->getHeight();
        int map_width = map->getWidth();

        for (int y = 0; y < map_height; ++y)
        {
            for (int x = 0; x < map_width; ++x)
            {
                float nx = static_cast<float>(x) / map_width * freq * res;
                float ny = static_cast<float>(y) / map_height * freq * res;
                float elevation = perlinNoise.normalizedOctave2D(nx, ny, 10) * 2;

                float latitude = static_cast<float>(y) / map_height;;
                float latitudeFactor = 1.0f - abs(2.0f * latitude - 1.0f);

                //Temperature And Humidity
                float elevationFactor = 1.0f - (elevation * elevation); 
                float temperature = (latitudeFactor * 0.75f + elevationFactor * 0.2f);

                float humidityNoise = perlinNoise.normalizedOctave2D(nx + 10.0f, ny + 10.0f, 10);
                float humidity = (latitudeFactor * 0.5f + elevationFactor * 0.3f + humidityNoise * 0.4f);

                GridCell cell(std::string(std::to_string(x) + "-" + std::to_string(y)), elevation, temperature, humidity);
                map->setCell(x, y, cell);
            }
        }
    }
} // namespace World
