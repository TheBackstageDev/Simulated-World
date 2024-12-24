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
        float HUDheight = (float)GetScreenHeight() * 1 / 4;
        float HUDwidth = (float)GetScreenWidth();

        float HUDy = GetScreenHeight() - HUDheight;

        int fontSize = static_cast<int>(HUDheight * 0.1f);

        Rectangle HUDrectangle{0, HUDy, HUDwidth, HUDheight};

        DrawRectangleRec(HUDrectangle, DARKGRAY);
        DrawRectangleLinesEx(HUDrectangle, 2.f, WHITE);

        int fps = GetFPS();

        int textPosX = 10;
        int textPosY = HUDy + 10;

        std::string timeFormatted = System::Time::getCurrentTimeFormatted();

        DrawText(TextFormat("FPS: %d", fps), (int)textPosX + (int)HUDwidth / 3, textPosY, fontSize, WHITE);
        DrawText(timeFormatted.c_str(), (int)textPosX + (int)HUDwidth / 2 + 100, textPosY - 2, fontSize, WHITE);

        float sliderWidth = HUDwidth * 0.1f;  
        float sliderHeight = HUDheight * 0.1f; 

        GuiSlider({textPosX + HUDwidth / 2 + 300, (float)textPosY, sliderWidth, sliderHeight}, "0.0001", "2.0", &World::SimulationStep, 0.0001f, 2.0f);

        float buttonX = HUDwidth - 150; 
        float buttonY = HUDy + 10;  
        if (GuiButton({buttonX, buttonY, 140, 30}, "Planet Stats"))
            isPlanetStatsOpen = !isPlanetStatsOpen;

        buttonX -= 150;
        if (GuiButton({buttonX, buttonY, 140, 30}, "Population Menu"))
            isPopMenuOpen = !isPopMenuOpen;

        buttonX -= 150;
        if (GuiButton({buttonX, buttonY, 140, 30}, "Civilization Menu"))
            isCivMenuOpen = !isCivMenuOpen;
    }

    static int searchedPopID = -1;
    static char popID[128] = "1";

    void InterfaceHandler::popDisplay()
    {
        static bool textBoxFocused = false; // Flag to track if the text box is focused

        if (isPopMenuOpen)
        {
            // Population menu implementation
            float menuWidth = GetScreenWidth() * 0.3f;
            float menuHeight = GetScreenHeight() * 0.4f;
            float menuX = (GetScreenWidth() - menuWidth) / 2;
            float menuY = (GetScreenHeight() - menuHeight) / 2;
            Rectangle menuArea = {menuX, menuY, menuWidth, menuHeight};
            DrawRectangleRec(menuArea, DARKGRAY);
            DrawRectangleLinesEx(menuArea, 2.f, WHITE);

            int fontSize = static_cast<int>(menuHeight * 0.04f);

            int textX = (int)menuX + 20;
            int textY = (int)menuY + 20;

            DrawText("Population Details", textX, textY, fontSize, WHITE);
            textY += fontSize + 10;
            DrawText(TextFormat("Total Global Population: %i", World::globalPopulation.size()), textX, textY, fontSize, WHITE);

            textY += fontSize + 10;

            // Check for focus on the text box
            Rectangle textBoxArea = {(float)textX, (float)textY, menuWidth - 40, 30};
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), textBoxArea))
            {
                textBoxFocused = true;
            }
            else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !CheckCollisionPointRec(GetMousePosition(), textBoxArea))
            {
                textBoxFocused = false;
            }

            if (textBoxFocused)
            {
                GuiTextBox(textBoxArea, popID, 128, true);
            }
            else
            {
                GuiTextBox(textBoxArea, popID, 128, false);
            }

            for (int i = 0; i < 128; i++)
            {
                if (!isdigit(popID[i]) && popID[i] != '\0')
                {
                    popID[i] = '\0'; 
                }
            }

            textY += 40;
            if (GuiButton({(float)textX, (float)textY, menuWidth - 40, 30}, "Search"))
            {
                searchedPopID = atoi(popID); 
                for (int i = 0; i < 128; ++i)
                {
                    popID[i] = '\0';
                }
            }
            textY += 50;

            if (searchedPopID > 0 && World::populationIDs.find(searchedPopID) != World::populationIDs.end())
            {
                auto &currentPop = System_Utils::getPop(searchedPopID);
                Vector2 currentPosition = currentPop->getPosition();
                Vector2 residencePosition = currentPop->getResidence()->getPos();
                DrawText(TextFormat("Person ID: %d", searchedPopID), textX, textY, fontSize, WHITE);
                textY += fontSize + 10;
                DrawText(TextFormat("Person Name: %s", currentPop->getName().c_str()), textX, textY, fontSize, WHITE);
                textY += fontSize + 10;
                DrawText(TextFormat("Person Age: %i", currentPop->getAge()), textX, textY, fontSize, WHITE);
                textY += fontSize + 10;
                DrawText(TextFormat("Person Food: %i", currentPop->getFoodAmmount()), textX, textY, fontSize, WHITE);
                textY += fontSize + 10;
                DrawText(TextFormat("Person Materials: %i", currentPop->getMaterialsAmmount()), textX, textY, fontSize, WHITE);
                textY += fontSize + 10;
                DrawText(TextFormat("Person Position: x: %i, y: %i", static_cast<int>(currentPosition.x), static_cast<int>(currentPosition.y)), textX, textY, fontSize, WHITE);
                textY += fontSize + 10;
                DrawText(TextFormat("Person Residence: x: %i, y: %i", static_cast<int>(residencePosition.x), static_cast<int>(residencePosition.y)), textX, textY, fontSize, WHITE);

                // 2nd Column
                textX += 250;  
                textY = menuY + 110 + 2 * (fontSize + 10); 

                DrawText(TextFormat("Person Health: %.2f", currentPop->getHealth()), textX, textY, fontSize, WHITE);
                textY += fontSize + 10;
                DrawText(TextFormat("Person Energy: %.2f", currentPop->getEnergy()), textX, textY, fontSize, WHITE);
                textY += fontSize + 10;
                DrawText(TextFormat("Person Gender: %s", currentPop->getGender().c_str()), textX, textY, fontSize, WHITE);
                textY += fontSize + 10;
                DrawText(TextFormat("Person's Partner: %s, ID: %i", currentPop->getPartner().c_str(), currentPop->getPartnerID()), textX, textY, fontSize, WHITE);
            }
        }
    }

    static int searchedCivID = -1;
    static char civID[128] = "1";

    void InterfaceHandler::civilizationDisplay()
    {
        static bool textBoxFocused = false; // Flag to track if the text box is focused

        if (isCivMenuOpen)
        {
            // Civilization menu implementation
            float menuWidth = GetScreenWidth() * 0.3f;
            float menuHeight = GetScreenHeight() * 0.4f;
            float menuX = (GetScreenWidth() - menuWidth) / 2;
            float menuY = (GetScreenHeight() - menuHeight) / 2;
            Rectangle menuArea = {menuX, menuY, menuWidth, menuHeight};
            DrawRectangleRec(menuArea, DARKGRAY);
            DrawRectangleLinesEx(menuArea, 2.f, WHITE);

            int fontSize = static_cast<int>(menuHeight * 0.04f);

            int textX = (int)menuX + 20;
            int textY = (int)menuY + 20;

            DrawText("Civilization Details", textX, textY, fontSize, WHITE);
            textY += fontSize + 10;
            DrawText(TextFormat("Total Civilizations: %i", World::civilizations.size()), textX, textY, fontSize, WHITE);

            textY += fontSize + 10;

            // Check for focus on the text box
            Rectangle textBoxArea = {(float)textX, (float)textY, menuWidth - 40, 30};
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), textBoxArea))
            {
                textBoxFocused = true;
            }
            else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !CheckCollisionPointRec(GetMousePosition(), textBoxArea))
            {
                textBoxFocused = false;
            }

            if (textBoxFocused)
            {
                GuiTextBox(textBoxArea, civID, 128, true);
            }
            else
            {
                GuiTextBox(textBoxArea, civID, 128, false);
            }

            for (int i = 0; i < 128; i++)
            {
                if (!isdigit(civID[i]) && civID[i] != '\0')
                {
                    civID[i] = '\0';
                }
            }

            textY += 40;
            if (GuiButton({(float)textX, (float)textY, menuWidth - 40, 30}, "Search"))
            {
                searchedCivID = atoi(civID);
                for (int i = 0; i < 128; ++i)
                {
                    civID[i] = '\0';
                }
            }
            textY += 50;

            if (searchedCivID > 0 && World::civilizations.find(searchedCivID) != World::civilizations.end())
            {
                auto &currentCiv = World::civilizations.at(searchedCivID);
                DrawText(TextFormat("Civ ID: %d", searchedCivID), textX, textY, fontSize, WHITE);
                textY += fontSize + 10;
                DrawText(TextFormat("Civ Name: %s", currentCiv.getName().c_str()), textX, textY, fontSize, WHITE);
                textY += fontSize + 10;
                DrawText(TextFormat("Civ Population: %i", currentCiv.getPopulation()), textX, textY, fontSize, WHITE);
                textY += fontSize + 10;
                DrawText(TextFormat("Civ Territory: %i", currentCiv.getTerritory().size()), textX, textY, fontSize, WHITE);
                textY += fontSize + 10;
                DrawText(TextFormat("Civ Food: %i", currentCiv.getFoodAmmount()), textX, textY, fontSize, WHITE);
                textY += fontSize + 10;
                DrawText(TextFormat("Civ Materials: %i", currentCiv.getMaterialsAmmount()), textX, textY, fontSize, WHITE);
            }
        }
    }

    void InterfaceHandler::menuDisplay()
    {
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

            int fontSize = static_cast<int>(buttonHeight * 0.025f);

            if (GuiButton({buttonX, buttonY, buttonWidth, (float)buttonHeight * fontSize}, "Political"))
            {
                World::currentDrawMode = World::drawMode::Political;
            }

            buttonY += buttonHeight + 10;

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
    }

    void InterfaceHandler::planetDisplay()
    {
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
            DrawText(TextFormat("North: %s", seasons[0].c_str()), (int)seasonTextPosX, (int)seasonTextPosY, 20, WHITE);
            DrawText(TextFormat("South: %s", seasons[1].c_str()), (int)seasonTextPosX, (int)seasonTextPosY + 30, 20, WHITE);

            float titleTextPosX = menuX + 10;
            float titleTextPosY = menuY + 10;
            DrawText("Planet Stats", (int)titleTextPosX, (int)titleTextPosY, 20, WHITE);
        }
    }

    static bool isCellSelected = false;
    static World::GridCell selectedCell;

    void InterfaceHandler::cellInfoDisplay()
    {
        float rectWidth = GetScreenWidth() * 0.25f;
        float rectHeight = GetScreenHeight() * 0.25f;
        float rectX = 0;
        float rectY = GetScreenHeight() - rectHeight;

        Rectangle cellInfoArea = {rectX, rectY, rectWidth, rectHeight};
        DrawRectangleLinesEx(cellInfoArea, 2.f, WHITE);

        if (isCellSelected)
        {
            int fontSize = static_cast<int>(rectHeight * 0.08f);

            // Collumn 1

            DrawText("CELL PROPERTIES", (int)cellInfoArea.x + 10, (int)cellInfoArea.y + 10, fontSize, RAYWHITE);
            DrawText(TextFormat("Name: %s", selectedCell.getName().c_str()), (int)cellInfoArea.x + 10, (int)cellInfoArea.y + 10 + fontSize, fontSize, RAYWHITE);
            DrawText(TextFormat("Elevation: %.2f", selectedCell.getElevation()), (int)cellInfoArea.x + 10, (int)cellInfoArea.y + 20 + fontSize * 2, fontSize, RAYWHITE);
            DrawText(TextFormat("Temperature: %.2f", selectedCell.getTemperature()), (int)cellInfoArea.x + 10, (int)cellInfoArea.y + 30 + fontSize * 3, fontSize, RAYWHITE);
            DrawText(TextFormat("Humidity: %.2f", selectedCell.getHumidity()), (int)cellInfoArea.x + 10, (int)cellInfoArea.y + 40 + fontSize * 4, fontSize, RAYWHITE);
            DrawText(TextFormat("Biome: %s", System_Utils::biomeToString(selectedCell.getBiome()).c_str()), (int)cellInfoArea.x + 10, (int)cellInfoArea.y + 50 + fontSize * 5, fontSize, RAYWHITE);
            DrawText(TextFormat("Materials: %i", selectedCell.getMaterialsAmmount()), (int)cellInfoArea.x + 10, (int)cellInfoArea.y + 60 + fontSize * 6, fontSize, RAYWHITE);
            DrawText(TextFormat("Food: %i", selectedCell.getFoodAmmount()), (int)cellInfoArea.x + 10, (int)cellInfoArea.y + 70 + fontSize * 7, fontSize, RAYWHITE);

            // Collumn 2

            DrawText(TextFormat("Civilization: %s", selectedCell.getCurrentCivilization() == -1 ? "None" : civilizations.at(selectedCell.getCurrentCivilization()).getName().c_str()), cellInfoArea.x + cellInfoArea.width / 2, cellInfoArea.y + fontSize, fontSize, RAYWHITE);
            DrawText(TextFormat("Population: %i", selectedCell.getPopulation()), cellInfoArea.x + cellInfoArea.width / 2, cellInfoArea.y + 10 + fontSize * 2, fontSize, RAYWHITE);
        }
    }

    void InterfaceHandler::runSimulationInterface()
    {
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

        civilizationDisplay();
        cellInfoDisplay();
        menuDisplay();
        planetDisplay();
        popDisplay();
    }

} // namespace Interface
