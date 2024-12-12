#include "../src/Headers/System/User_Interface/InterfaceHandler.hpp"
#include "../src/Headers/System/Map/WorldGeneration.hpp"
#include "../src/Headers/Utils/WindowUtils.hpp"
#include "../src/Headers/Utils/CameraUtils.hpp"
#include "../src/Headers/Utils/WorldDefinitions.hpp"

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
            auto &camera = System_Utils::getCam();
            auto *map = World::WorldGenerator::getMap();

            float cellSize = map->getCellSize();
            float scale = System_Utils::getCurrentWindowScaleOffset({map->getWidth() * cellSize, map->getHeight() * cellSize}) * camera.zoom;

            Vector2 mousePos = GetMousePosition();
            Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);

            // Does the inverse scaling that the drawMap() function does
            worldPos = (worldPos - camera.offset) / scale + camera.target;

            if (worldPos.x >= 0 && worldPos.x < map->getWidth() * cellSize &&
                worldPos.y >= 0 && worldPos.y < map->getHeight() * cellSize)
            {
                selectedCell = World::WorldGenerator::getGridCellAtPos(worldPos);
                isCellSelected = true;
            }
            else
            {
                isCellSelected = false; // Deselect cell if clicked outside the map
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE))
            isMenuOpen = !isMenuOpen;

        if (isMenuOpen)
        {
            float menuWidth = GetScreenWidth() * 0.3f;
            float menuHeight = GetScreenHeight() * 0.4f;
            float menuX = (GetScreenWidth() - menuWidth) / 2;
            float menuY = (GetScreenHeight() - menuHeight) / 2;
            Rectangle menuArea = {menuX, menuY, menuWidth, menuHeight};
            DrawRectangleRec(menuArea, DARKGRAY);

            int buttonHeight = 40;
            float buttonX = menuX + 20;
            float buttonY = menuY + 30;
            float buttonWidth = menuWidth - 40;

            if (GuiButton({buttonX, buttonY, buttonWidth, (float)buttonHeight}, "Terrain"))
            {
                World::currentDrawMode = World::drawMode::Terrain;
            }

            buttonY += buttonHeight + 10;
            if (GuiButton({buttonX, buttonY, buttonWidth, (float)buttonHeight}, "Elevation"))
            {
                World::currentDrawMode = World::drawMode::Elevation;
            }

            buttonY += buttonHeight + 10;
            if (GuiButton({buttonX, buttonY, buttonWidth, (float)buttonHeight}, "Biomes"))
            {
                World::currentDrawMode = World::drawMode::Biomes;
            }

            buttonY += buttonHeight + 10;
            if (GuiButton({buttonX, buttonY, buttonWidth, (float)buttonHeight}, "Humidity"))
            {
                World::currentDrawMode = World::drawMode::Humidity;
            }

            buttonY += buttonHeight + 10;
            if (GuiButton({buttonX, buttonY, buttonWidth, (float)buttonHeight}, "Temperature"))
            {
                World::currentDrawMode = World::drawMode::Temperature;
            }

            buttonY += buttonHeight + 20;
            if (GuiButton({buttonX, buttonY, buttonWidth, (float)buttonHeight}, "Return"))
            {
                isMenuOpen = false;
            }

            return;
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
