#include "../src/Headers/System/Simulation.hpp"

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

            EndDrawing();
        }
    }

    void Simulation::runSimulation()
    {

    }
} // namespace System
