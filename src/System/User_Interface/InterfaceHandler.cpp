#include "../src/Headers/System/User_Interface/InterfaceHandler.hpp"
#include "../src/Headers/System/Map/WorldGeneration.hpp"
#include "../src/Headers/Utils/WindowUtils.hpp"
#include "../src/Headers/Utils/CameraUtils.hpp"

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

namespace Interface
{
    SimulationMode InterfaceHandler::currentSimulationMode = Menu;

    InterfaceHandler::InterfaceHandler() {}

    void InterfaceHandler::runInterface()
    {
        if (IsWindowResized() || IsWindowMaximized())
            System_Utils::updateScales();

        switch (currentSimulationMode)
        {
        case Menu:
            menu.runMenu();
            if (menu.getCurrentMode() == 2)
                currentSimulationMode = Running;
            break;
        case Running:
            runSimulationInterface();
            break;
        default:
            break;
        }
    }

    void InterfaceHandler::runSimulationInterface()
    {
        static bool isCellSelected = false;
        static World::GridCell selectedCell;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            auto& camera = System_Utils::getCam();
            auto* map = World::WorldGenerator::getMap();

            float cellSize = map->getCellSize();
            float scale = System_Utils::getCurrentWindowScaleOffset({map->getWidth() * cellSize, map->getHeight() * cellSize}) * camera.zoom;

            Vector2 mousePos = GetMousePosition();
            Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);

            //Does the inverse scaling that the drawMap() function does
            worldPos = (worldPos - camera.offset) / scale + camera.target;

            selectedCell = World::WorldGenerator::getGridCellAtPos(worldPos);
            isCellSelected = true;
        }

        // Display the properties of the selected cell
        if (isCellSelected && !selectedCell.getName().empty())
        {
            Rectangle cellInfoArea = {10, GetScreenHeight() / 2 + 20, GetScreenWidth() - 20, GetScreenHeight() / 2 - 30};
            DrawRectangle(cellInfoArea.x, cellInfoArea.y, cellInfoArea.width, cellInfoArea.height, DARKGRAY);
            DrawText("CELL PROPERTIES", cellInfoArea.x + 10, cellInfoArea.y + 10, 20, RAYWHITE);
            DrawText(TextFormat("Name: %s", selectedCell.getName().c_str()), cellInfoArea.x + 10, cellInfoArea.y + 40, 20, RAYWHITE);
            DrawText(TextFormat("Elevation: %.2f", selectedCell.getElevation()), cellInfoArea.x + 10, cellInfoArea.y + 70, 20, RAYWHITE);
            DrawText(TextFormat("Temperature: %.2f", selectedCell.getTemperature()), cellInfoArea.x + 10, cellInfoArea.y + 100, 20, RAYWHITE);
            DrawText(TextFormat("Humidity: %.2f", selectedCell.getHumidity()), cellInfoArea.x + 10, cellInfoArea.y + 130, 20, RAYWHITE);
        }
    }

} // namespace Interface
