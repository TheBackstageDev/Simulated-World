#pragma once

#include <string>
#include <random>
#include "../src/Headers/System/Map/WorldCell.hpp"
#include "../src/Headers/System/Time.hpp"

namespace Simulation_AI
{
    class Pop
    {
    private:
        //Resourcesy
        uint32_t materialsAmmount{0};
        uint32_t foodAmmount{0};

        //Personalebjh
        std::string name;
        char32_t age{0};
        uint32_t birthDate;
        World::GridCell* residence{nullptr};
        std::vector<System::Event> history;
        
        uint32_t id;

        //Stats
        float max_health{100}; // Will Decrease with wounds or Age
        float health{100};
        float energy{100};

        Vector2 position;

        // Civilization
        bool isLeader{false};
        uint32_t civilization; // Which civilization does the Pop belong to

        //Helpers
        void updateHealth(float increment);
        void isCellAdequate(World::GridCell &cell);
        bool isCellMoveable(World::GridCell &cell);
        Vector2 lookForCell();

        // AI Behaviour
        void move(Vector2 cell);
        void migrate(Vector2 newCell);
        void manageCivilization();

        // AI Flags

        //Migration Flags
        bool isLookingForNewResidence{false};
        bool isNewCellAdequate{false};

    public:
        Pop(std::string name = "John Doe", char32_t age = 0, Vector2 position = {0, 0}, uint32_t civilization = 0);
        ~Pop() = default;

        void simulate();

        //Getters
        std::string getName() const { return name; }
        char32_t getAge() const { return age; }
        uint32_t getID() const { return id; }
        World::GridCell *getResidence() const { return residence; }
        Vector2 getPosition() const { return position; }
        std::vector<System::Event> getPersonalHistory() const { return history; }

        //Setters

        void setLeader(uint32_t leaderCivilization) { isLeader = true; civilization = leaderCivilization; }
    };

} // namespace Simulation_AI
