#include "../src/Headers/Utils/WorldDefinitions.hpp"

namespace World
{
    drawMode currentDrawMode{drawMode::Terrain};
    float SimulationStep{1.0f};
    
    std::unordered_map<uint32_t, Simulation_AI::Pop> globalPopulation;
    std::unordered_map<uint32_t, Simulation_AI::Civilization> civilizations;
} // namespace World
