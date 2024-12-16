#pragma once

#include <string>
#include "../src/Headers/System/Map/WorldCell.hpp"
#include "../src/Headers/System/Time.hpp"

namespace Simulation
{
    class Pop
    {
    private:
        //Resources
        uint32_t materialsAmmount{0};
        uint32_t foodAmmount{0};

        //Personalg
        std::string name;
        char32_t age{0};
        World::GridCell* residence{nullptr};
        std::vector<System::Event> history;

        //Stats
        char16_t health{100};
        char16_t energy{100};
    public:
        Pop(std::string name, char32_t age = 0);
        ~Pop();

        //Getters
        std::string getName() const { return name; }
        char32_t getAge() const { return age; }
        World::GridCell *getResidence() const { return residence; }
        std::vector<System::Event> getPersonalHistory() const { return history; }
    };
    
} // namespace Simulation
