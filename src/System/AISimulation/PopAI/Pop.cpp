#include "../src/Headers/System/AISimulation/PopAI/Pop.hpp"

namespace Simulation_AI
{
    static uint32_t lastID{0};
    Pop::Pop(std::string name, char32_t age)
    : name(name), age(age), id(++lastID)
    {
    }
} // namespace Simulation_AI
