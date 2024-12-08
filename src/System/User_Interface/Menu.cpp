#include "../src/Headers/System/User_Interface/Menu.hpp"
#include "../src/Headers/Utils/WindowUtils.hpp"

#include <random>

namespace Interface
{
    MenuInterface::MenuInterface() {}

    void MenuInterface::runMenu()
    {
        while (!WindowShouldClose())
        {
            if (IsWindowResized() || IsWindowMaximized())
                System_Utils::updateScales();

            BeginDrawing();
            ClearBackground(BLACK);
            handleInput();
            if (currentMode == 0)
            {
                drawMenu();
            }
            else if (currentMode == 1)
            {
                drawLobby();
            }
            EndDrawing();
        }
    }

    void MenuInterface::drawMenu()
    {
        Vector2 screenCenter = System_Utils::getScreenCenter();
        float buttonWidth = 200;
        float buttonHeight = 50;

        float centerX = screenCenter.x - buttonWidth / 2;
        float centerY = screenCenter.y - (buttonHeight * 2 + 20) / 2;
        
        DrawText("Simulation", centerX, centerY - 100, 40, RAYWHITE);
        if (GuiButton({centerX, centerY, buttonWidth, buttonHeight}, "Start New Simulation"))
        {
            currentMode = 1; 
        }

        if (GuiButton({centerX, centerY + 50, buttonWidth, buttonHeight}, "Exit"))
        {
            CloseWindow();
        }
    }

    void MenuInterface::drawLobby()
    {
        drawMapDisplay();

        float mapWidth = GetScreenWidth() - 20;
        float mapHeight = GetScreenHeight() / 2;
        float settingsWidth = mapWidth;
        float settingsHeight = GetScreenHeight() / 2 - 20;
        Rectangle settingsArea = {10, mapHeight + 20, settingsWidth, settingsHeight};
        DrawRectangleLines(settingsArea.x, settingsArea.y, settingsArea.width, settingsArea.height, RAYWHITE);
        DrawText("SETTINGS", settingsArea.x + 10, settingsArea.y + 10, 20, RAYWHITE);

        static float res = 0.5f;              
        static float freq = 0.5f;            
        static char seedBuffer[32] = "90254";

        GuiLabel({settingsArea.x + 10, settingsArea.y + 40, 50 * System_Utils::scaleX, 20 * System_Utils::scaleY}, "RES");
        GuiSlider({settingsArea.x + 70, settingsArea.y + 40, 200 * System_Utils::scaleX, 20 * System_Utils::scaleY}, "0.0", "15.0", &res, 0.0f, 15.0f);
        GuiLabel({settingsArea.x + 170, settingsArea.y + 40, 50 * System_Utils::scaleX, 20 * System_Utils::scaleY}, TextFormat(" %.1f", res));

        GuiLabel({settingsArea.x + 10, settingsArea.y + 70, 50 * System_Utils::scaleX, 20 * System_Utils::scaleY}, "FREQ");
        GuiSlider({settingsArea.x + 70, settingsArea.y + 70, 200 * System_Utils::scaleX, 20 * System_Utils::scaleY}, "0.0", "15.0", &freq, 0.0f, 15.0f);
        GuiLabel({settingsArea.x + 170, settingsArea.y + 70, 50 * System_Utils::scaleX, 20 * System_Utils::scaleY}, TextFormat("%.1f", freq));

        GuiLabel({settingsArea.x + 10, settingsArea.y + 100, 50 * System_Utils::scaleX, 20 * System_Utils::scaleY}, "SEED");
        GuiTextBox({settingsArea.x + 70, settingsArea.y + 100, 100 * System_Utils::scaleX, 20 * System_Utils::scaleY}, seedBuffer, sizeof(seedBuffer), true);

        if (GuiButton({settingsArea.x + 10, settingsArea.y + 140, 150, 30}, "GENERATE"))
        {
            int seed = atoi(seedBuffer);
        }

        if (GuiButton({settingsArea.x + 170, settingsArea.y + 140, 150, 30}, "ENTER"))
        {
        }

        if (GuiButton({20, (float)GetScreenHeight() - 40, 100, 30}, "Back to Menu"))
        {
            currentMode = 0; 
        }
    }

    void MenuInterface::drawMapDisplay()
    {
        // Draw the map area
        float mapWidth = GetScreenWidth() - 20;
        float mapHeight = GetScreenHeight() / 2;
        Rectangle mapArea = {10, 10, mapWidth, mapHeight};
        DrawRectangleLines(mapArea.x, mapArea.y, mapArea.width, mapArea.height, RAYWHITE);
        DrawText("MAP", mapArea.x + mapArea.width / 2 - MeasureText("MAP", 20) / 2, mapArea.y + mapArea.height / 2 - 10, 20, RAYWHITE);
    }

    void MenuInterface::drawGenerationSettings(Rectangle settingsBox)
    {
        float centerX = settingsBox.x + settingsBox.width / 2;
        float centerY = settingsBox.y + settingsBox.height / 2;
    }

    void MenuInterface::handleInput()
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            CloseWindow();
        }
    }
    
} // namespace Interface
