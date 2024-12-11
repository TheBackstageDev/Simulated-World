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
        void runSimulationInterface();
        SimulationMode getCurrentMode() { return currentSimulationMode; }

        bool isMenuOpen{false};
    };

} // namespace Interface
