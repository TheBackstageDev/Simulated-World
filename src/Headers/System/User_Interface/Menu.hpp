#pragma once

#include <raygui.h>

namespace Interface
{
    class MenuInterface
    {
    private:
        void drawMenu();

        void drawMapDisplay();
        void drawGenerationSettings(Rectangle settingsBox);

        void drawPlanetInterface(Vector2 pos, float eccentricity, float distance, float axialTilt);
        void drawLobby();
        void handleInput();

        char16_t currentMode{0}; //0: Menu, 1: Lobby
    public:
        MenuInterface();

        void runMenu();
        char16_t getCurrentMode() { return currentMode; }
    };
} // namespace Interface
