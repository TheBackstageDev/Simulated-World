#include "../src/Headers/System/Simulation.hpp"

#include <iostream>

namespace System
{
    Simulation::Simulation()
    {
        std::random_device rd;
        int seed = rd();

        const int CellSize = 5;

        WorldGen.GenerateWorld(seed, 10, 5, 5);

        camera.target = {0.0f, 0.0f};
        camera.offset = {0.0f, 0.0f};
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;

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
        if (IsKeyDown(KEY_W))
            camera.target.y -= 10 / camera.zoom;
        if (IsKeyDown(KEY_S))
            camera.target.y += 10 / camera.zoom;
        if (IsKeyDown(KEY_A))
            camera.target.x -= 10 / camera.zoom;
        if (IsKeyDown(KEY_D))
            camera.target.x += 10 / camera.zoom;

        camera.zoom += GetMouseWheelMove() * 0.1f;
        if (camera.zoom < 0.1f)
            camera.zoom = 0.1f;

        WorldGen.renderMap(camera);
    }
} // namespace System
