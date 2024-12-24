#include "../src/Headers/Utils/WorldDefinitions.hpp"

namespace World
{
    float orbitalPeriod{365.f};
    float maxAgeIncrease{0.f};

    drawMode currentDrawMode{drawMode::Political};
    float SimulationStep{1.0f};
    float deltaSimulationTime{0.0f};

    std::set<uint32_t> populationIDs{};
    std::unordered_map<uint32_t, std::shared_ptr<Simulation_AI::Pop>> globalPopulation{};
    std::unordered_map<uint32_t, Simulation_AI::Civilization> civilizations{};
} // namespace World
