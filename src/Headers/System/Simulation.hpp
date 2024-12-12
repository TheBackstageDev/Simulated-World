#pragma once

#include <raylib.h>
#include "./Map/WorldGeneration.hpp"
#include "Time.hpp"
#include "./User_Interface/InterfaceHandler.hpp"

namespace System
{
    class Simulation
    {
    private:
        World::WorldGenerator WorldGen{};
        Interface::InterfaceHandler interfaceHand{};
        Time timeController{};
    public:
        Simulation();
        ~Simulation();

        void init();
        void runSimulation();
    };
} // namespace System
