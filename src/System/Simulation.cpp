#include "../src/Headers/System/Simulation.hpp"
#include "../src/Headers/System/InputHandler.hpp"
#include "../src/Headers/System/User_Interface/InterfaceHandler.hpp"
#include "../src/Headers/Utils/WindowUtils.hpp"
#include "../src/Headers/Utils/CameraUtils.hpp"
#include "../src/Headers/Utils/WorldDefinitions.hpp"

#include <iostream>

namespace System
{
    Simulation::Simulation()
    {
        System_Utils::cam.target = {0.0f, 0.0f};
        System_Utils::cam.offset = {0.0f, 0.0f};
        System_Utils::cam.rotation = 0.0f;
        System_Utils::cam.zoom = 2.0f;

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
        static float deltaSimulationTime{0.0f};

        deltaSimulationTime += GetFrameTime();

        if (deltaSimulationTime > World::SimulationStep)
        {
            timeController.advanceTime();
            deltaSimulationTime = 0.0f;

            WorldGen.getPlanet()->updatePlanet();
        }

        if (interfaceHand.getCurrentMode() == Interface::SimulationMode::Running)
        {
            System_Input::handleCameraInput(System_Utils::cam);

            BeginMode2D(System_Utils::cam);
            WorldGen.renderMap(System_Utils::cam);
            EndMode2D();
        }

        interfaceHand.runInterface();
    }
} // namespace System
