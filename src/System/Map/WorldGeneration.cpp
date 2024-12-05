#include "../src/Headers/System/Map/WorldGeneration.hpp"

namespace World
{
    WorldGenerator::WorldGenerator()
    {
    }

    WorldGenerator::~WorldGenerator() {}

    void WorldGenerator::GenerateWorld(int seed, int res, int freq, int CellSize)
    {
        perlinNoise = siv::PerlinNoise(seed);
        map = new WorldMap(CellSize);

        int map_height = map->getHeight();
        int map_width = map->getWidth();

        for (int y = 0; y < map_height; ++y)
        {
            for (int x = 0; x < map_width; ++x)
            {
                float nx = static_cast<float>(x) / map_width * freq;
                float ny = static_cast<float>(y) / map_height * freq;
                float elevation = perlinNoise.octave2D(nx, ny, 10);
                GridCell cell(std::string(std::to_string(x) + std::to_string(y)), elevation);
                map->setCell(x, y, cell);
            }
        }
    }
} // namespace World
