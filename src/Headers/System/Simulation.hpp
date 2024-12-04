#pragma once

#include <raylib.h>

namespace System
{
    class Simulation
    {
    private:
        
    public:
        Simulation();
        ~Simulation();

        void init();
        void runSimulation();
    };
} // namespace System
