#pragma once

#include <raylib.h>
#include "./Map/WorldGeneration.hpp"

namespace System
{
    class Simulation
    {
    private:
        World::WorldGenerator WorldGen{};
        Camera2D camera{0};
    public:
        Simulation();
        ~Simulation();

        void init();
        void runSimulation();
    };
} // namespace System
