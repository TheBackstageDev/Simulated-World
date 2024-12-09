#include "../src/Headers/System/User_Interface/InterfaceHandler.hpp"
#include "../src/Headers/Utils/WindowUtils.hpp"

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
        default:
            break;
        }
    }
} // namespace Interface
