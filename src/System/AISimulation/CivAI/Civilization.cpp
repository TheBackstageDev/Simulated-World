#include "../src/Headers/System/AISimulation/CivAI/Civilization.hpp"
#include "../src/Headers/Utils/WorldUtils.hpp"

namespace Simulation_AI
{
    static uint32_t lastID{0};

    Civilization::Civilization(
        uint32_t leaderID,
        Color civColor,
        Vector2 rootCell,
        uint32_t population,
        std::string name,
        GovernmentType governmentType)
        : leader(leaderID), civColor(civColor), population(population), id(++lastID), name(name), governmentType(governmentType)
    {
        cellsOwned.push_back(rootCell);
        World::WorldGenerator::getGridCellAtPos(rootCell).setCivilizationOwnership(this->id);
    }

    void Civilization::expand(Vector2 cell)
    {
        auto &currentCell = World::WorldGenerator::getGridCellAtPos(cell);

        cellsOwned.push_back(cell);
        currentCell.setCivilizationOwnership(this->id);
    }
} // namespace Simulation_AI
