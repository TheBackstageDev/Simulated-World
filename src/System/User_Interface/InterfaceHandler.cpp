#include "../src/Headers/System/User_Interface/InterfaceHandler.hpp"
#include "../src/Headers/Utils/WindowUtils.hpp"

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

namespace Interface
{
    InterfaceHandler::InterfaceHandler() {}

    void InterfaceHandler::runInterface()
    {
        if (IsWindowResized() || IsWindowMaximized())
            System_Utils::updateScales();
            
        menu.runMenu();
    }
} // namespace Interface
