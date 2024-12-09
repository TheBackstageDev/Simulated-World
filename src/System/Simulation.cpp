#include "../src/Headers/System/Simulation.hpp"
#include "../src/Headers/System/InputHandler.hpp"
#include "../src/Headers/System/User_Interface/InterfaceHandler.hpp"
#include "../src/Headers/Utils/WindowUtils.hpp"
#include "../src/Headers/Utils/WorldDefinitions.hpp"

#include <iostream>

namespace System
{
    Simulation::Simulation()
    {
        camera.target = {0.0f, 0.0f};
        camera.offset = {0.0f, 0.0f};
        camera.rotation = 0.0f;
        camera.zoom = 2.0f;

        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(System_Utils::windowWidth, System_Utils::windowHeight, "Simulation");
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
        if (interfaceHand.getCurrentMode() == Interface::SimulationMode::Running)
        {
            System_Input::handleCameraInput(camera);

            BeginMode2D(camera);
            WorldGen.renderMap(camera);
            EndMode2D();
        }

        interfaceHand.runInterface();
    }
} // namespace System
