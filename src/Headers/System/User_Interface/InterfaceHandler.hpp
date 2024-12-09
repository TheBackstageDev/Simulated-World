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

        static SimulationMode currentSimulationMode;
    public:
        InterfaceHandler();

        void runInterface();
        SimulationMode getCurrentMode() { return currentSimulationMode; }
    };

} // namespace Interface