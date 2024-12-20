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

        void popDisplay();
        void civilizationDisplay();
        void planetDisplay();
        void menuDisplay();
        void cellInfoDisplay();
        void headsUpDisplay();

        bool isMenuOpen{false};
        bool isPopMenuOpen{false};
        bool isCivMenuOpen{false};
    public:
        InterfaceHandler();

        void runInterface();
        void runSimulationInterface();
        SimulationMode getCurrentMode() { return currentSimulationMode; }
    };

} // namespace Interface
