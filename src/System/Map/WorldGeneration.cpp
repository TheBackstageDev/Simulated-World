#include "../src/Headers/System/Map/WorldGeneration.hpp"

#include <cassert>
#include <cmath>

namespace World
{
    WorldMap *World::WorldGenerator::map = nullptr;
    Planet* WorldGenerator::planet = nullptr;
    siv::PerlinNoise World::WorldGenerator::perlinNoise{0};

    WorldGenerator::WorldGenerator()
    {
    }

    WorldGenerator::~WorldGenerator() 
    {
        delete map;
        delete planet;
    }

    void WorldGenerator::GenerateRivers()
    {
        int map_height = map->getHeight();
        int map_width = map->getWidth();
        int riverCount = 0;

        const float startElevationThreshold = 0.8f; // Height at which rivers will start forming
        const float maxElevationThreshold = 1.2f;   // Max height for river formation

        auto &mountainPeaks = map->getMountainPeaks();
        std::vector<GridCell> peaks;

        while (!mountainPeaks.empty())
        {
            GridCell p = mountainPeaks.top();
            mountainPeaks.pop();

            peaks.push_back(p);
        }

        for (const auto &source : peaks)
        {
            if (riverCount >= MAX_RIVERS)
                return;

            Vector2 currentPosition = source.getPos();
            int length = 0;
            std::vector<GridCell*> riverPath; // Track the cells modified to form the river

            bool reachedOcean = false;

            while (length < MAX_RIVER_LENGTH)
            {
                auto &currentCell = map->getCell(currentPosition.x, currentPosition.y);

                if (currentCell.getBiome() == Biome::Ocean)
                {
                    reachedOcean = true;
                    break;
                }

                riverPath.push_back(&currentCell);

                Vector2 lowestPos = currentCell.getPos();
                float lowestElevation = currentCell.getElevation();
                Vector2 sameElevationPos = currentCell.getPos();
                bool foundSameElevation = false;

                for (int dx = -1; dx <= 1; ++dx)
                {
                    for (int dy = -1; dy <= 1; ++dy)
                    {
                        int neighborX = currentPosition.x + dx;
                        int neighborY = currentPosition.y + dy;
                        if (neighborX >= 0 && neighborX < map_width && neighborY >= 0 && neighborY < map_height)
                        {
                            auto &neighborCell = map->getCell(neighborX, neighborY);    
                            
                            if (neighborCell.getElevation() < lowestElevation + .01f)
                            {
                                lowestPos = neighborCell.getPos();
                                lowestElevation = neighborCell.getElevation();
                            }
                            else if (neighborCell.getElevation() == currentCell.getElevation())
                            {
                                sameElevationPos = neighborCell.getPos();
                                foundSameElevation = true;
                            }
                        }
                    }
                }

                if (lowestPos.x == currentPosition.x && lowestPos.y == currentPosition.y && foundSameElevation)
                    lowestPos = sameElevationPos;

                if (lowestPos.x == currentPosition.x && lowestPos.y == currentPosition.y)
                    break; // No lower or same elevation neighbor found

                currentPosition = lowestPos;
                ++length;
            }

            if (reachedOcean)
            {
                for (auto& cell : riverPath)
                {
                    cell->updateElevation(-.02f);
                    cell->setBiome(Biome::River);
                }

                ++riverCount;
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

        float hillFreq = freq * 0.5f;
        float mountainFreq = freq * 0.5f;

        auto& mountainPeaks = map->getMountainPeaks();

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
                    elevationFactor = 1.0f - (elevation * elevation);
                
                float temperature = 1 - (planet->getSolarEnergy((latitude * 180.f) - 90.f) * (latitudeFactor * 0.75f + elevationFactor * 0.2f));

                float humidityNoise = perlinNoise.normalizedOctave2D(nx + 10.0f, ny + 10.0f, 10);
                float humidity = (latitudeFactor * 0.4f + elevationFactor * 0.3f + humidityNoise * 0.4f);

                humidity = std::clamp(humidity, 0.0f, 1.0f);

                //Is Ocean so its naturally Humid
                if (elevation < SEA_LEVEL && temperature > 0.3f)
                    humidity += 0.05f;

                GridCell cell(std::string(std::to_string(x) + "-" + std::to_string(y)), {(float)x, (float)y}, elevation, temperature, humidity);

                if (elevation > MOUNTAIN_PEAK_MIN_HEIGHT && isPeakDistantFromOthers(mountainPeaks, cell.getPos()))
                    mountainPeaks.push(cell);
                

                map->setCell(x, y, cell);
            }
        }

        planet->updatePlanet();

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
