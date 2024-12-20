#include "../src/Headers/System/AISimulation/CivAI/Civilization.hpp"
#include "../src/Headers/Utils/WorldUtils.hpp"

namespace Simulation_AI
{
    static uint32_t lastID;
    
    Civilization::Civilization(Pop &leader, Color civColor, Vector2 rootCell, uint32_t population, std::string name) 
    : leader(leader), civColor(civColor), population(population), id(lastID++), name(name)
    {
        cellsOwned.push_back(rootCell);
        World::WorldGenerator::getGridCellAtPos(rootCell).setCivilizationOwnership(this->id);
    }

    void Civilization::expand(Vector2 cell)
    {
        float cost = System_Utils::calculateCellCost(cell);
        auto &currentCell = World::WorldGenerator::getGridCellAtPos(cell);

        if (materialsAmmount < cost && currentCell.getCurrentCivilization() != -1)
            return; //Later to add more complex behaviour

        materialsAmmount -= cost;
        cellsOwned.push_back(cell);
        currentCell.setCivilizationOwnership(this->id);
    }
} // namespace Simulation_AI
