#pragma once

#include <raylib.h>
#include <string>

#include "../src/Headers/System/Time.hpp"
#include "../src/Headers/System/AISimulation/PopAI/Pop.hpp"

enum class GovernmentType
{
    Monarchy,
}; // Placeholder

namespace Simulation_AI
{
    class Civilization
    {
    private:
        //Economy
        float taxRate{20.f};
        uint32_t materialsAmmount{0};
        uint32_t foodAmmount{0};

        //Demographics
        uint32_t population{1}; // 1 since atleast The leader will be living here
        uint32_t size{1}; // 1 since atleast One tile it'll have in the beggining

        //Politics
        Pop& leader;
        GovernmentType governmentType;
        Color civColor;
        std::vector<Vector2> cellsOwned{};

        //Other
        std::string name;
        uint32_t lifeTime{0}; // in Years
        uint32_t id;

        void expand(Vector2 cell);

        friend class Pop;
    public:
        Civilization(
            Pop &leader, 
            Color civColor, 
            Vector2 rootCell, 
            uint32_t population = 1, 
            std::string name = "Default Civ", 
            GovernmentType governmentType = GovernmentType::Monarchy);

        // Getters
        std::string& getName() { return name; }
        uint32_t getID() const { return id; }
        Color getColor() const { return civColor; }
        float getTaxRate() const { return taxRate / 100; } //gets the percentage 
        uint32_t getMaterialsAmmount() const { return materialsAmmount; }
        uint32_t getFoodAmmount() const { return foodAmmount; }
        uint32_t getPopulation() const { return population; }

        Pop& getLeader() { return leader; }
        std::vector<Vector2> getTerritory() { return cellsOwned; }
        Vector2 getCapital() { return cellsOwned[0]; }
        GovernmentType getGovernmentType() { return governmentType; }

        //Setters

        void setLeader(Pop &newLeader) { leader = newLeader; }
        void incrementFood(uint32_t increment) { foodAmmount += increment; }
        void incrementMaterials(uint32_t increment) { materialsAmmount += increment; }
        void incrementPopulation(uint32_t increment) { population += increment; }
    };
} // namespace Simulation_AI
