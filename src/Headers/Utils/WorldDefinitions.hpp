#pragma once

#include <unordered_map>
#include <set>
#include <memory>
#include "../src/Headers/System/AISimulation/CivAI/Civilization.hpp"
#include "../src/Headers/System/AISimulation/PopAI/Pop.hpp"
#include "../src/Headers/System/Map/WorldGeneration.hpp"

namespace World
{
    //Height Defines
    #define SEA_LEVEL 0.05f

    //WorldMap Defines
    static float Cellsize{1.0f}; // in KM
    #define MAX_RIVERS 10
    #define MAX_RIVER_LENGTH 25 * (1 / Cellsize) // in KM
    #define MAX_RIVER_WIDTH 2

    #define MOUNTAIN_PEAKS_MIN_DISTANCE 30 * (1 / Cellsize) // in KM
    #define MOUNTAIN_PEAK_MIN_HEIGHT 1.0

    #define ARCTIC_TEMP 0.08f

    //GridCell Definitions
    #define MAX_COST 1000.f

    //Planet Defines
    #define SOLAR_CONSTANT 1361.0f // Solar constant in W/m²
    #define UPDATE_RATE 30 // In Days

    extern float orbitalPeriod;
    extern float maxAgeIncrease;

    //Simulation Defines
    #define GRAVITATIONAL_CONSTANT 6.67408e-11
    #define STAR_MASS 1.989e30 // Equivalent value to 1 * Mass of the Sun

    extern drawMode currentDrawMode;
    extern float SimulationStep;
    extern float deltaSimulationTime;

    //Civilization Defines
    #define POP_DAWN_AMMOUNT 100
    #define POP_DAWN_AGE 18

    struct comparePop
    {
        bool operator()(const std::shared_ptr<Simulation_AI::Pop> &lhs, const std::shared_ptr<Simulation_AI::Pop> &rhs) const
        {
            return lhs->getID() < rhs->getID();
        }
    };

    extern std::unordered_map<uint32_t, Simulation_AI::Civilization> civilizations;
    extern std::set<uint32_t> populationIDs;
    extern std::set<std::shared_ptr<Simulation_AI::Pop>, comparePop> globalPopulation;
} // namespace System_Utils
