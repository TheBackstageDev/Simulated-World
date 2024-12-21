#include "../src/Headers/Utils/WorldUtils.hpp"
#include "../src/Headers/Utils/WorldDefinitions.hpp"

namespace System_Utils
{
    std::string System_Utils::biomeToString(Biome biome)
    {
        switch (biome)
        {
        case Biome::Ocean:
            return "Ocean";
        case Biome::River:
            return "River";
        case Biome::Dirt:
            return "Dirt";
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

    bool System_Utils::isPeakDistantFromOthers(std::priority_queue<GridCell, std::vector<GridCell>, CompareGridCell> mountainPeaks, Vector2 peak)
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

    float System_Utils::calculateCellCost(Vector2 cell)
    {
        auto &currentCell = WorldGenerator::getGridCellAtPos(cell);

        float cost = 0.f;

        uint32_t resourcesAmmount = currentCell.getMaterialsAmmount() + currentCell.getFoodAmmount();
        cost += 10.f / (resourcesAmmount + 1);
        cost += currentCell.getElevation() * 0.1f;

        switch (currentCell.getBiome())
        {
        case Biome::Grassland:
            cost += 1.f;
            break;
        case Biome::RainForest:
            cost += 5.f;
            break;
        case Biome::Desert:
        case Biome::Mountain:
            cost += 10.f;
            break;
        case Biome::Hill:
            cost += 7.f;
            break;
        case Biome::Forest:
        case Biome::River:
        case Biome::Ocean:
            cost += 2.f;
            break;
        default:
            cost += 1.f;
            break;
        }

        cost = std::clamp(cost, 0.f, MAX_COST);

        return cost;
    }

    GridCell &System_Utils::getCell(Vector2 cell) { return WorldGenerator::getGridCellAtPos(cell); }

    Simulation_AI::Civilization *getCiv(uint32_t civID) { return &World::civilizations.at(civID); }

    std::shared_ptr<Simulation_AI::Pop> System_Utils::getPop(uint32_t popID)
    {
        if (World::globalPopulation.find(popID) != World::globalPopulation.end())
            return World::globalPopulation.at(popID);

        return nullptr;
    }
}