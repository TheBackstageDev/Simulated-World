#pragma once

#include <raylib.h>
#include <string>

#include "../src/Headers/System/Time.hpp"
#include "../src/Headers/System/Map/WorldMap.hpp"
#include "../src/Headers/System/AISimulation/PopAI/Pop.hpp"
#include <map>

namespace Simulation
{
    static std::vector<Civilization> civilizations;

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

        //Otherb
        uint32_t lifeTime{0}; // in Years
        uint32_t id;

        void expand();

        Civilization(Pop &leader, Color civColor, uint32_t population = 1, uint32_t id);
    public:
        static Civilization createCivilization(Pop &leader, Color civColor, uint32_t population = 1)
        {
            static uint32_t lastID{0};
            Civilization civ(leader, civColor, population, ++lastID);
            civilizations.push_back(std::move(civ));
            return civ;
        }
    };
} // namespace Simulation_AI
