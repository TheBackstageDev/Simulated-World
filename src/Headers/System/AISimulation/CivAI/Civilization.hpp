#pragma once

#include <raylib.h>
#include <string>

#include "../src/Headers/System/Time.hpp"
#include "../src/Headers/System/AISimulation/PopAI/Pop.hpp"

namespace Simulation_AI
{
    class Civilization
    {
    private:
        //Economy
        char16_t taxRate{10};
        uint32_t materialsAmmount{0};
        uint32_t foodAmmount{0};

        //Demographics
        uint32_t population{1}; // 1 since atleast The leader will be living here
        uint32_t size{1}; // 1 since atleast One tile it'll have in the beggining

        //Politics
        Pop& leader;
        Color civColor;
        std::vector<Vector2> cellsOwned;

        //Other
        std::string name;
        uint32_t lifeTime{0}; // in Years
        uint32_t id;

        void expand(Vector2 cell);
    public:
        Civilization(Pop &leader, Color civColor, uint32_t population = 1, std::string name = "Default Civ");

        void placeCivilization(Vector2 rootCell);

        std::string& getName() { return name; }
        uint32_t getId() { return id; }
    };
} // namespace Simulation_AI
