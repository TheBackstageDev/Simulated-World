#pragma once

#include "Menu.hpp"

namespace Interface
{
    enum SimulationMode
    {
        Menu,
        Running
    };

    class InterfaceHandler
    {
    private:
        MenuInterface menu{};

        SimulationMode currentMode;
    public:
        InterfaceHandler();

        void runInterface();
        SimulationMode getCurrentMode() { return currentMode; }
    };

} // namespace Interface
