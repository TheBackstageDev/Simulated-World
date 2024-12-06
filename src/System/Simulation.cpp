#include "../src/Headers/System/Simulation.hpp"
#include "../src/Headers/System/InputHandler.hpp"

#include <iostream>

namespace System
{
    Simulation::Simulation()
    {
        std::random_device rd;
        int seed = rd();

        const float CellSize = 1.f;

        WorldGen.GenerateWorld(seed, 10, 5, CellSize);

        camera.target = {0.0f, 0.0f};
        camera.offset = {0.0f, 0.0f};
        camera.rotation = 0.0f;
        camera.zoom = 2.0f;

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
        System_Input::handleCameraInput(camera);

        BeginMode2D(camera);
        WorldGen.renderMap(camera);
        EndMode2D();
    }
} // namespace System
