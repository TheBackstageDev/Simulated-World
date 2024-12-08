#pragma once

#include <raylib.h>
#include "./Map/WorldGeneration.hpp"
#include "./User_Interface/InterfaceHandler.hpp"

namespace System
{
    class Simulation
    {
    private:
        World::WorldGenerator WorldGen{};
        Interface::InterfaceHandler interfaceHand{};

        Camera2D camera{0};
    public:
        Simulation();
        ~Simulation();

        void init();
        void runSimulation();
    };
} // namespace System
