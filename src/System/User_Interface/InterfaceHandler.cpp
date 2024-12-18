#include "../src/Headers/System/User_Interface/InterfaceHandler.hpp"
#include "../src/Headers/System/Map/WorldGeneration.hpp"
#include "../src/Headers/Utils/WindowUtils.hpp"
#include "../src/Headers/Utils/CameraUtils.hpp"
#include "../src/Headers/Utils/WorldDefinitions.hpp"
#include "../src/Headers/Utils/WorldUtils.hpp"
#include "../src/Headers/System/Time.hpp"
#include "../src/Headers/Utils/InputDefinitions.hpp"

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

    bool isPlanetStatsOpen{false};

    void InterfaceHandler::headsUpDisplay()
    {
        float HUDheight = GetScreenHeight() * 1 / 4;
        float HUDwidth = GetScreenWidth();

        float HUDy = GetScreenHeight() - HUDheight;

        int fontSize = static_cast<int>(HUDheight * 0.1f);

        Rectangle HUDrectangle{0, HUDy, HUDwidth, HUDheight};

        DrawRectangleRec(HUDrectangle, DARKGRAY);
        DrawRectangleLinesEx(HUDrectangle, 2.f, WHITE);

        int fps = GetFPS();

        float textPosX = 10;
        float textPosY = HUDy + 10;

        std::string timeFormatted = System::Time::getCurrentTimeFormatted();

        DrawText(TextFormat("FPS: %d", fps), textPosX + HUDwidth / 2, textPosY, fontSize, WHITE);
        DrawText(timeFormatted.c_str(), textPosX + HUDwidth / 2 + 100, textPosY - 2, fontSize, WHITE);

        float sliderWidth = HUDwidth * 0.1f;  
        float sliderHeight = HUDheight * 0.1f; 

        GuiSlider({textPosX + HUDwidth / 2 + 300, textPosY, sliderWidth, sliderHeight}, "0.01", "2.0", &World::SimulationStep, 0.01f, 2.0f);

        float buttonX = HUDwidth - 150; 
        float buttonY = HUDy + 10;  
        if (GuiButton({buttonX, buttonY, 140, 30}, "Planet Stats"))
        {
            isPlanetStatsOpen = !isPlanetStatsOpen;
        }
    }

    void InterfaceHandler::runSimulationInterface()
    {
        static bool isCellSelected = false;
        static World::GridCell selectedCell;

        if (IsMouseButtonPressed(SELECT_CELL))
        {
            auto &camera = System_Utils::cam;
            auto *map = World::WorldGenerator::getMap();

            float cellSize = map->getCellSize();
            Vector2 worldPos = System_Utils::getRealCellMousePosition(camera);

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

        headsUpDisplay();

        if (IsKeyPressed(OPEN_MENU))
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

            int fontSize = static_cast<int>(buttonHeight * 0.03f);

            if (GuiButton({buttonX, buttonY, buttonWidth, (float)buttonHeight * fontSize}, "Terrain"))
            {
                World::currentDrawMode = World::drawMode::Terrain;
            }

            buttonY += buttonHeight + 10;
            if (GuiButton({buttonX, buttonY, buttonWidth, (float)buttonHeight * fontSize}, "Elevation"))
            {
                World::currentDrawMode = World::drawMode::Elevation;
            }

            buttonY += buttonHeight + 10;
            if (GuiButton({buttonX, buttonY, buttonWidth, (float)buttonHeight * fontSize}, "Biomes"))
            {
                World::currentDrawMode = World::drawMode::Biomes;
            }

            buttonY += buttonHeight + 10;
            if (GuiButton({buttonX, buttonY, buttonWidth, (float)buttonHeight * fontSize}, "Humidity"))
            {
                World::currentDrawMode = World::drawMode::Humidity;
            }

            buttonY += buttonHeight + 10;
            if (GuiButton({buttonX, buttonY, buttonWidth, (float)buttonHeight * fontSize}, "Temperature"))
            {
                World::currentDrawMode = World::drawMode::Temperature;
            }

            buttonY += buttonHeight + 10;
            if (GuiButton({buttonX, buttonY, buttonWidth, (float)buttonHeight * fontSize}, "Population"))
            {
                World::currentDrawMode = World::drawMode::Population;
            }

            buttonY += buttonHeight + 20;
            if (GuiButton({buttonX, buttonY, buttonWidth, (float)buttonHeight * fontSize}, "Return"))
            {
                isMenuOpen = false;
            }

            return;
        }

        float rectWidth = GetScreenWidth() * 0.25f;
        float rectHeight = GetScreenHeight() * 0.25f;
        float rectX = 0;
        float rectY = GetScreenHeight() - rectHeight;

        Rectangle cellInfoArea = {rectX, rectY, rectWidth, rectHeight};
        DrawRectangleLinesEx(cellInfoArea, 2.f, WHITE);

        if (isCellSelected)
        {
            int fontSize = static_cast<int>(rectHeight * 0.08f);

            //Collumn 1

            DrawText("CELL PROPERTIES", cellInfoArea.x + 10, cellInfoArea.y + 10, fontSize, RAYWHITE);
            DrawText(TextFormat("Name: %s", selectedCell.getName().c_str()), cellInfoArea.x + 10, cellInfoArea.y + 10 + fontSize, fontSize, RAYWHITE);
            DrawText(TextFormat("Elevation: %.2f", selectedCell.getElevation()), cellInfoArea.x + 10, cellInfoArea.y + 20 + fontSize * 2, fontSize, RAYWHITE);
            DrawText(TextFormat("Temperature: %.2f", selectedCell.getTemperature()), cellInfoArea.x + 10, cellInfoArea.y + 30 + fontSize * 3, fontSize, RAYWHITE);
            DrawText(TextFormat("Humidity: %.2f", selectedCell.getHumidity()), cellInfoArea.x + 10, cellInfoArea.y + 40 + fontSize * 4, fontSize, RAYWHITE);
            DrawText(TextFormat("Biome: %s", System_Utils::biomeToString(selectedCell.getBiome()).c_str()), cellInfoArea.x + 10, cellInfoArea.y + 50 + fontSize * 5, fontSize, RAYWHITE);
            DrawText(TextFormat("Materials: %i", selectedCell.getMaterialsAmmount()), cellInfoArea.x + 10, cellInfoArea.y + 60 + fontSize * 6, fontSize, RAYWHITE);
            DrawText(TextFormat("Food: %i", selectedCell.getFoodAmmount()), cellInfoArea.x + 10, cellInfoArea.y + 70 + fontSize * 7, fontSize, RAYWHITE);

            //Collumn 2

            DrawText(TextFormat("Civilization: %s", selectedCell.getCurrentCivilization() == -1 ? "None" : civilizations.at(selectedCell.getCurrentCivilization()).getName().c_str()), cellInfoArea.x + cellInfoArea.width / 2 + 10, cellInfoArea.y + 10, fontSize, RAYWHITE);
        }

        if (isPlanetStatsOpen)
        {
            float menuWidth = GetScreenWidth() * 0.3f;
            float menuHeight = GetScreenHeight() * 0.4f;
            float menuX = (GetScreenWidth() - menuWidth) / 2;
            float menuY = (GetScreenHeight() - menuHeight) / 2;
            Rectangle menuArea = {menuX, menuY, menuWidth, menuHeight};
            DrawRectangleRec(menuArea, DARKGRAY);
            DrawRectangleLinesEx(menuArea, 2.0f, WHITE);

            float planetRadius = 10.0f; 
            float planetPosX = menuX + menuWidth / 4;
            float planetPosY = menuY + menuHeight / 2;

            World::Planet *planet = World::WorldGenerator::getPlanet();
            planet->drawPlanetOrbitDepiction({planetPosX, planetPosY}, planetRadius, planet->getOrbitalEccentricity(), planet->getDistanceFromStar());

            std::array<std::string, 2> seasons = planet->getSeasons();

            float depictionPosX = menuX + 3 * menuWidth / 4;
            planet->drawPlanetDepiction({depictionPosX, planetPosY}, 30.f, planet->getAxialTilt());

            float seasonTextPosX = depictionPosX - 100;
            float seasonTextPosY = planetPosY + 80;
            DrawText(TextFormat("North: %s", seasons[0].c_str()), seasonTextPosX, seasonTextPosY, 20, WHITE);
            DrawText(TextFormat("South: %s", seasons[1].c_str()), seasonTextPosX, seasonTextPosY + 30, 20, WHITE);

            float titleTextPosX = menuX + 10;
            float titleTextPosY = menuY + 10;
            DrawText("Planet Stats", titleTextPosX, titleTextPosY, 20, WHITE);
        }
    }

} // namespace Interface
