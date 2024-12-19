#include "../src/Headers/Utils/WorldDefinitions.hpp"

namespace World
{
    float orbitalPeriod{365.f};

    drawMode currentDrawMode{drawMode::Political};
    float SimulationStep{1.0f};
    
    std::unordered_map<uint32_t, Simulation_AI::Pop> globalPopulation;
    std::unordered_map<uint32_t, Simulation_AI::Civilization> civilizations;
} // namespace World
