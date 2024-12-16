#include "../src/Headers/System/AISimulation/CivAI/Civilization.hpp"

namespace Simulation
{
    std::vector<Civilization> Simulation::civilizations;

    Civilization::Civilization(Pop &leader, Color civColor, uint32_t population, uint32_t id) 
    : leader(leader), civColor(civColor), population(population), id(id)
    {
    }
} // namespace Simulation
