#pragma once

#include <string>
#include <random>
#include "../src/Headers/System/Map/WorldCell.hpp"
#include "../src/Headers/System/Time.hpp"

#define ADULT_AGE 18
#define TEEN_AGE 13

#define DEFAULT_NAME "Mr Nobody"

namespace Simulation_AI
{
    enum class PopGender
    {
        None,
        Male,
        Female,
    };

    class Pop 
    {
    private:
        //Resourcesy
        uint32_t materialsAmmount{0};
        uint32_t foodAmmount{0};

        //Personal
        std::string name{DEFAULT_NAME};
        PopGender gender{PopGender::None};
        char32_t age{0};
        char32_t max_age{100};
        uint32_t birthDate{0};
        World::GridCell* residence{nullptr};
        std::vector<System::Event> history{};

        std::vector<uint32_t> children{};
        std::vector<uint32_t> parents{0, 0}; // First will always be the Father, second always the Mother
        uint32_t partner{0};

        uint32_t id{0};

        //Stats
        float max_health{100}; // Will Decrease with wounds or Age
        float health{100};
        float energy{100};

        Vector2 position{-1, -1};

        // Civilization
        bool isLeader{false};
        uint32_t civilization{0}; // Which civilization does the Pop belong to

        //Helpers
        void updateHealth(float increment);
        void updateEnergy(float increment);
        void updateFood(float increment);
        void updateMaterial(float increment);
        bool isCellAdequate(World::GridCell &cell);
        bool isCellMoveable(World::GridCell &cell);
        Vector2 lookForCell();
        uint32_t lastAgeUpdateTime{0};
        World::GridCell& getCurrentCell();

        void handleDeath();
        void handleDeathLogging();
        void updateTime();

        // AI Behaviour
        void move(Vector2 cell);
        void migrate(Vector2 newCell);
        void rest();
        void gatherFood();
        void gatherMaterial();

        // Related to Social Interactions
        void socialize();
        void reproduce();
        void haveChild();

        //RNG
        void event();

        // Civilization Management 
        void handleLeaderDeath();
        void manageCivilization();
        void expandCivilization();

        // Other
        void nameGenerator(std::string fatherName, std::string motherName, bool newSurname);

        void addEventToHistory(const std::string event)
        {
            System::Event newEvent(System::Time::getCurrentTime(), event, id);
            history.push_back(newEvent);
        }
        
        // AI Flags

        uint32_t timePregnant{0};
        bool isPregnant{false};

        //Migration Flags
        bool isLookingForNewResidence{false};
        bool isNewCellAdequate{false};

    public:
        Pop(std::string name = "John Doe", char32_t age = 0, Vector2 position = {0, 0}, uint32_t civilization = 0, PopGender gender = PopGender::None);
        ~Pop();

        void simulate();

        //Getters
        std::string getName() const { return name; }
        std::string isPopPregnant() const { return isPregnant ? "Yes" : "No"; }
        std::string getGender() const 
        {
            switch (gender)
            {
            case PopGender::Male:
                return "Male";
            case PopGender::Female:
                return "Female";
            default:
                return "Attack Helicopter Boeing AH-64 Apache";
                break;
            }
        }
        std::string getPartner();
        uint32_t getPartnerID() const 
        {
            return partner;
        }
        char32_t getAge() const { return age; }
        uint32_t getID() const { return id; }
        uint32_t getFoodAmmount() const { return foodAmmount; }
        uint32_t getMaterialsAmmount() const { return materialsAmmount; }
        float getHealth() const { return health; }
        float getEnergy() const { return energy; }
        World::GridCell *getResidence() const { return residence; }
        Vector2 getPosition() const { return position; }
        std::vector<System::Event> getPersonalHistory() const { return history; }

        //Setters
        void createNewName(std::string fatherName, std::string motherName, bool newSurname = false) { nameGenerator(fatherName, motherName, newSurname); }
        void setPartner(uint32_t partner) { this->partner = partner; }
        void setLeader(uint32_t leaderCivilization) 
        { 
            this->isLeader = true; 
            this->civilization = leaderCivilization; 
            this->addEventToHistory("Became the Leader of the Civilization!");
        }
    };

} // namespace Simulation_AI
