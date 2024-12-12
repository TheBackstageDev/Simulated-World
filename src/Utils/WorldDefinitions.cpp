#include "../src/Headers/Utils/WorldDefinitions.hpp"
#include <cmath>

namespace World
{
    drawMode currentDrawMode{drawMode::Terrain};
    float SimulationStep{1.0f};

    std::string World::biomeToString(Biome biome)
    {
        switch (biome)
        {
        case Biome::Ocean:
            return "Ocean";
        case Biome::River:
            return "River";
        case Biome::Beach:
            return "Beach";
        case Biome::Forest:
            return "Forest";
        case Biome::RainForest:
            return "RainForest";
        case Biome::Desert:
            return "Desert";
        case Biome::Mountain:
            return "Mountain";
        case Biome::Hill:
            return "Hill";
        case Biome::Tundra:
            return "Tundra";
        case Biome::Grassland:
            return "Grassland";
        case Biome::Savanna:
            return "Savanna";
        case Biome::Arctic:
            return "Arctic";
        default:
            return "Unknown Biome";
        }
    }

    bool World::isPeakDistantFromOthers(std::priority_queue<GridCell, std::vector<GridCell>, CompareGridCell> mountainPeaks, Vector2 peak)
    {
        std::vector<GridCell> peaks; 

        while (!mountainPeaks.empty())
        {
            GridCell p = mountainPeaks.top();
            mountainPeaks.pop();

            peaks.push_back(p);
        }

        for (const auto &p : peaks)
        {
            Vector2 otherPeak = p.getPos();
            float distance = std::sqrt(std::pow(peak.x - otherPeak.x, 2) + std::pow(peak.y - otherPeak.y, 2));

            if (distance < MOUNTAIN_PEAKS_MIN_DISTANCE)
            {
                return false; // The peak is not distant enough from others
            }
        }

        return true; 
    }

} // namespace World
