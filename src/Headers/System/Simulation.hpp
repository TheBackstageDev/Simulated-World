#pragma once

#include <raylib.h>
#include "./Map/WorldGeneration.hpp"
#include "Time.hpp"
#include "./User_Interface/InterfaceHandler.hpp"
#include "../src/Headers/System/SystemBackend/ThreadPool.hpp"

namespace System
{
    class Simulation
    {
    private:
        World::WorldGenerator WorldGen{};
        Interface::InterfaceHandler interfaceHand{};
        Time timeController{};

        System_Inner::ThreadPool AIPool{1};

        void runSimulation();
        void runAISimulation();
        void beginCivilizations();
    public:
        Simulation();
        ~Simulation();

        void init();
    };
} // namespace System
