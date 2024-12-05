#include "../src/Headers/System/Simulation.hpp"

#include <iostream>

namespace System
{
    Simulation::Simulation()
    {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(800, 500, "Simulation");
    }

    Simulation::~Simulation()
    {
        CloseWindow();
    }

    void Simulation::init()
    {
        while (!WindowShouldClose())
        {
            BeginDrawing();
            ClearBackground(BLACK);

            runSimulation();

            EndDrawing();
        }
    }

    void Simulation::runSimulation()
    {
        if (!WorldGen.doesMapExist())
        {
            std::random_device rd;
            int seed = rd();

            WorldGen.GenerateWorld(seed, 5, 5);
        }

        WorldGen.renderMap();
    }
} // namespace System
