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

        if (isCellSelected && !selectedCell.getName().empty())
        {
            float rectWidth = GetScreenWidth() * 0.25f;
            float rectHeight = GetScreenHeight() * 0.25f;
            float rectX = 10;
            float rectY = GetScreenHeight() - rectHeight - 10;

            Rectangle cellInfoArea = {rectX, rectY, rectWidth, rectHeight};
            DrawRectangle(cellInfoArea.x, cellInfoArea.y, cellInfoArea.width, cellInfoArea.height, DARKGRAY);

            // Calculate font size based on the rectangle height
            int fontSize = static_cast<int>(rectHeight * 0.1f);

            DrawText("CELL PROPERTIES", cellInfoArea.x + 10, cellInfoArea.y + 10, fontSize, RAYWHITE);
            DrawText(TextFormat("Name: %s", selectedCell.getName().c_str()), cellInfoArea.x + 10, cellInfoArea.y + 10 + fontSize, fontSize, RAYWHITE);
            DrawText(TextFormat("Elevation: %.2f", selectedCell.getElevation()), cellInfoArea.x + 10, cellInfoArea.y + 20 + fontSize * 2, fontSize, RAYWHITE);
            DrawText(TextFormat("Temperature: %.2f", selectedCell.getTemperature()), cellInfoArea.x + 10, cellInfoArea.y + 30 + fontSize * 3, fontSize, RAYWHITE);
            DrawText(TextFormat("Humidity: %.2f", selectedCell.getHumidity()), cellInfoArea.x + 10, cellInfoArea.y + 40 + fontSize * 4, fontSize, RAYWHITE);
            DrawText(TextFormat("Biome: %s", World::biomeToString(selectedCell.getBiome()).c_str()), cellInfoArea.x + 10, cellInfoArea.y + 50 + fontSize * 5, fontSize, RAYWHITE);
        }
    }

} // namespace Interface
