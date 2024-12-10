#include "../src/Headers/System/User_Interface/Menu.hpp"
#include "../src/Headers/System/Map/WorldGeneration.hpp"
#include "../src/Headers/Utils/WindowUtils.hpp"

#include <random>

namespace Interface
{
    MenuInterface::MenuInterface() {}

    Texture2D mapTexture = {0};

    void MenuInterface::runMenu()
    {
        while (!WindowShouldClose() && currentMode < 2)
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

        UnloadTexture(mapTexture);
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

    static bool seedInitialized = false;

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

        static float res{0.5f};
        static float freq{0.5f};
        static char seedBuffer[32];

        if (!seedInitialized)
        {
            std::random_device rd;
            snprintf(seedBuffer, sizeof(seedBuffer), "%d", rd());
            seedInitialized = true;
        }

        GuiLabel({settingsArea.x + 10, settingsArea.y + 40, 50 * System_Utils::scaleX, 20 * System_Utils::scaleY}, "CELL SIZE");
        GuiSlider({settingsArea.x + 70, settingsArea.y + 40, 200 * System_Utils::scaleX, 20 * System_Utils::scaleY}, "0.1", "10.0", &System_Utils::Cellsize, 0.0f, 15.0f);
        GuiLabel({settingsArea.x + 170, settingsArea.y + 40, 50 * System_Utils::scaleX, 20 * System_Utils::scaleY}, TextFormat(" %.1f", System_Utils::Cellsize));

        GuiLabel({settingsArea.x + 10, settingsArea.y + 70, 50 * System_Utils::scaleX, 20 * System_Utils::scaleY}, "RES");
        GuiSlider({settingsArea.x + 70, settingsArea.y + 70, 200 * System_Utils::scaleX, 20 * System_Utils::scaleY}, "0.0", "15.0", &res, 0.0f, 15.0f);
        GuiLabel({settingsArea.x + 170, settingsArea.y + 70, 50 * System_Utils::scaleX, 20 * System_Utils::scaleY}, TextFormat(" %.1f", res));

        GuiLabel({settingsArea.x + 10, settingsArea.y + 100, 50 * System_Utils::scaleX, 20 * System_Utils::scaleY}, "FREQ");
        GuiSlider({settingsArea.x + 70, settingsArea.y + 100, 200 * System_Utils::scaleX, 20 * System_Utils::scaleY}, "0.0", "15.0", &freq, 0.0f, 15.0f);
        GuiLabel({settingsArea.x + 170, settingsArea.y + 100, 50 * System_Utils::scaleX, 20 * System_Utils::scaleY}, TextFormat("%.1f", freq));

        GuiLabel({settingsArea.x + 10, settingsArea.y + 130, 50 * System_Utils::scaleX, 20 * System_Utils::scaleY}, "SEED");
        GuiTextBox({settingsArea.x + 70, settingsArea.y + 130, 100 * System_Utils::scaleX, 20 * System_Utils::scaleY}, seedBuffer, sizeof(seedBuffer), true);

        if (GuiButton({settingsArea.x + 10, settingsArea.y + 200, 150, 30}, "GENERATE"))
        {
            int seed = atoi(seedBuffer);

            World::WorldGenerator::GenerateWorld(seed, res, freq, System_Utils::Cellsize);

            if (mapTexture.id != 0)
                UnloadTexture(mapTexture);

            Image mapImg = World::WorldGenerator::getMapImage();
            mapTexture = LoadTextureFromImage(mapImg);
            UnloadImage(mapImg);
        }

        if (GuiButton({settingsArea.x + 170, settingsArea.y + 200, 150, 30}, "ENTER") && mapTexture.id != 0)
        {
            currentMode = 2;
        }

        if (GuiButton({20, (float)GetScreenHeight() - 40, 100, 30}, "Back to Menu"))
        {
            currentMode = 0;

            if (mapTexture.id != 0)
                UnloadTexture(mapTexture);
        }
    }

    void MenuInterface::drawMapDisplay()
    {
        // Draw the map area
        float mapWidth = GetScreenWidth() - 20;
        float mapHeight = GetScreenHeight() / 2 - 20;
        Rectangle mapArea = {10, 10, mapWidth, mapHeight};
        DrawRectangleLines(mapArea.x, mapArea.y, mapArea.width, mapArea.height, RAYWHITE);

        if (mapTexture.id != 0)
        {
            float textureWidth = mapTexture.width;
            float textureHeight = mapTexture.height;

            float scale = System_Utils::getScaleOffset({mapArea.width, mapArea.height} ,{textureHeight, textureHeight});

            textureWidth *= scale;
            textureHeight *= scale;

            float posX = mapArea.x + (mapArea.width - textureWidth) / 2;
            float posY = mapArea.y + (mapArea.height - textureHeight) / 2;

            DrawTextureEx(mapTexture, {posX, posY}, 0.0f, scale, RAYWHITE);
            DrawRectangleLines(posX, posY, textureWidth, textureHeight, RAYWHITE);
        }
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
