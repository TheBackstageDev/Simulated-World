#include "../src/Headers/System/Simulation.hpp"
#include "../src/Headers/System/AISimulation/CivAI/Civilization.hpp"
#include "../src/Headers/System/InputHandler.hpp"
#include "../src/Headers/System/User_Interface/InterfaceHandler.hpp"
#include "../src/Headers/Utils/WindowUtils.hpp"
#include "../src/Headers/Utils/CameraUtils.hpp"
#include "../src/Headers/Utils/WorldDefinitions.hpp"
#include "../src/Headers/Utils/WorldUtils.hpp"

#include <iostream>

using namespace Simulation_AI;

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

    void Simulation::runAISimulation()
    {
        for (auto& [id, pop] : globalPopulation)
        {
            AIPool.enqueue([&]{ pop.simulate(); });
        }
    }

    void Simulation::beginCivilizations()
    {
        Vector2 CellPos = System_Utils::getRealCellMousePosition(System_Utils::cam);
        auto &currentCell = System_Utils::getCell(CellPos);
        
        Pop leader("King Adam", POP_DAWN_AGE, currentCell.getPos());

        Civilization dawnCiv(leader, RED, CellPos, POP_DAWN_AMMOUNT, "Adawnia");
        civilizations.emplace(dawnCiv.getId(), std::move(dawnCiv));

        leader.setLeader(dawnCiv.getId());
        globalPopulation.emplace(leader.getID(), std::move(leader));

        for (int i = 0; i < POP_DAWN_AMMOUNT - 1; i++)
        {
            Pop newPop("Johanes Doe", POP_DAWN_AGE, currentCell.getPos(), dawnCiv.getId());
            globalPopulation.emplace(newPop.getID(), std::move(newPop));
        }

        currentCell.updatePopulation(POP_DAWN_AMMOUNT);
        timeController.logEvent("First Civilization Rises!");
    }

    void Simulation::runSimulation()
    {
        deltaSimulationTime += GetFrameTime();

        if (interfaceHand.getCurrentMode() == Interface::SimulationMode::Running)
        {
            System_Input::handleCameraInput(System_Utils::cam);
            bool result = System_Input::handleSimulationInput(System_Utils::cam);

            if (result && civilizations.size() == 0)
                beginCivilizations();

            BeginMode2D(System_Utils::cam);
            WorldGen.renderMap(System_Utils::cam);
            EndMode2D();
        }

        if (deltaSimulationTime > World::SimulationStep)
        {
            timeController.advanceTime();
            deltaSimulationTime = 0.0f;

            runAISimulation();
            WorldGen.getPlanet()->updatePlanet();
        }

        interfaceHand.runInterface();
    }
} // namespace System
