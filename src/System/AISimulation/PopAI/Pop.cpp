#include "../src/Headers/System/AISimulation/PopAI/Pop.hpp"
#include "../src/Headers/Utils/WorldUtils.hpp"

#include <cmath>
#include <random>
#include <cassert> 
#include <mutex>

namespace Simulation_AI
{
    static uint32_t lastID{0};
    Pop::Pop(std::string name, char32_t age, Vector2 position, uint32_t civilization, PopGender gender)
    : name(name), age(age), id(++lastID), position(position), civilization(civilization), birthDate(System::Time::getCurrentTime())
    {
        residence = &System_Utils::getCell(position);
        residence->movePop(this->id, true);

        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> max_ageRand(25, 55);

        max_age = max_ageRand(rng) + World::maxAgeIncrease;

        if (gender == PopGender::None) // To Decide now
        {
            std::uniform_int_distribution<int> genderRand(1,2);
            this->gender = (PopGender)genderRand(rng);
        }
        else
        {
            this->gender = gender;
        }

        if (this->gender == PopGender::Female)
            this->name = "Jane doe";

        addEventToHistory("Has been bringed into Existence!");
    }

    Pop::~Pop() {}

    std::string Pop::getPartner() const
    {
        if (partner != 0)
        {
            auto& partner = System_Utils::getPop(this->partner);
            return partner->getName();
        }

        return "None";
    }

    void Pop::handleLeaderDeath()
    {
    }

    void Pop::handleDeathLogging()
    {
        if (partner != 0)
        {
            auto &this_partner = System_Utils::getPop(this->partner);
            this_partner->partner = 0;
            this_partner->addEventToHistory("Lost a partner!, " + this->getName());
        }

        if (parents.size() != 0)
        {
            for (const auto &parent : this->parents)
            {
                auto &pop = System_Utils::getPop(parent);
                pop->addEventToHistory("Lost a child!, " + this->getName());
            }
        }

        if (children.size() != 0)
        {
            for (const auto &children : this->children)
            {
                auto &child = System_Utils::getPop(children);
                child->addEventToHistory("Lost a parent!, " + this->getName());

                if (this->gender == PopGender::Male)
                    child->parents.at(0) = 0;
                else
                    child->parents.at(1) = 0;
            }
        }
    }

    void Pop::handleDeath()
    {
        if (health <= 0 || age > max_age)
        {
            if (residence != nullptr)
            {
                residence->updatePopulation(-1);
                getCurrentCell().movePop(id, false);

                this->residence = nullptr;
            } 
            else
            {
                getCurrentCell().movePop(id, false);
            }

            auto* civ = System_Utils::getCiv(civilization);
            civ->incrementPopulation(-1);

            if (isLeader)
                handleLeaderDeath();

            handleDeathLogging();

            System::Time::logPopEvent(history, id);
            globalPopulation.erase(id);
        }
    }

    void Pop::updateHealth(float increment)
    {
        health += increment;
        health = std::clamp(health, 0.0f, max_health);
    }
    void Pop::updateEnergy(float increment)
    {
        energy += increment;
        energy = std::clamp(energy, 0.0f, 100.f);
    }
    void Pop::updateFood(float increment)
    {
        auto civ = System_Utils::getCiv(civilization);
        float tax = civ->getTaxRate();

        uint32_t ammountTaxed = floor(increment * tax);
        civ->incrementFood(ammountTaxed);

        foodAmmount += increment - ammountTaxed;
    }

    Vector2 Pop::lookForCell()
    {
        std::mt19937 rng(std::random_device{}());

        std::uniform_int_distribution<int> offset_dist(-1, 1);
        int randXoffset = offset_dist(rng);
        int randYoffset = offset_dist(rng);

        Vector2 currentCellPos = residence->getPos();

        return { currentCellPos.x + randXoffset, currentCellPos.y + randYoffset };
    }

    World::GridCell &Pop::getCurrentCell()
    {
        return System_Utils::getCell(position);
    }

    bool Pop::isCellMoveable(GridCell &cell)
    {
        if (cell.getPos().x != residence->getPos().x || cell.getPos().y != residence->getPos().y)
        {
            Biome currentBiome = cell.getBiome();

            if (currentBiome == Biome::Arctic || currentBiome == Biome::Ocean || currentBiome == Biome::Dirt)
                return false;

            return true;
        }
        
        return false;
    }

    bool Pop::isCellAdequate(GridCell &cell)
    {
        // Check if the cell is not the current residence
        if (isCellMoveable(cell))
        {
            bool optimalTemperature = cell.getTemperature() >= 0.3 && cell.getTemperature() <= 0.9;
            bool optimalHumidity = cell.getHumidity() <= 0.8;

            if (optimalTemperature && optimalHumidity)
            {
                isNewCellAdequate = true;
                return true;
            }
        }

        return false;
    }

    // AI Behaviour

    void Pop::move(Vector2 cell)
    {
        std::mutex mtx;
        std::lock_guard<std::mutex> lock(mtx);

        auto& oldCell = getCurrentCell();
        oldCell.movePop(id, false);

        position = cell;
        auto &currentCell = getCurrentCell();

        currentCell.movePop(id, true);

        updateEnergy(-5.f);

        isCellAdequate(currentCell);
    }

    void Pop::migrate(Vector2 newCell)
    {
        auto& newCellRef = System_Utils::getCell(newCell);

        if (newCellRef.getCurrentCivilization() != civilization && health > max_health * .45f)
            return; // Prefer staying inside the nation

        if (residence != nullptr)
            residence->updatePopulation(-1);

        residence = &System_Utils::getCell(newCell);
        move(newCell);
        residence->updatePopulation(+1);

        if (partner != 0)
        {
            auto& this_partner = System_Utils::getPop(this->partner);
            this_partner->residence->updatePopulation(-1);
            this_partner->residence = this->residence;
            residence->updatePopulation(+1);
        }
        //Resets Flags
        isNewCellAdequate = false;
        isLookingForNewResidence = false;
    }

    void Pop::gatherFood()
    {
        if (isLeader)
            return; // Leader won't gather Resources

        auto civ = System_Utils::getCiv(civilization);
        auto& currentCell = System_Utils::getCell(position);

        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> delta_reources(0, 3);

        std::uniform_int_distribution<int> event_happen(1, 10);
        int eventWillHappen = event_happen(rng);

        if (eventWillHappen == 1)
            event(); // event will happen while gathering resources;

        int gatheredFood = delta_reources(rng);
        
        updateFood((float)gatheredFood);

        currentCell.updateFood(-gatheredFood);
        updateEnergy(-5.f);
    }

    void Pop::rest()
    {
        if (isLeader) // Leader Actions
        {
            auto civ = System_Utils::getCiv(civilization);
            if (civ->getFoodAmmount() > 5 && energy < 60.f) // Leaders eat more since they have more.
            {
                civ->incrementFood(-5);
                updateEnergy(60.f);
                updateHealth(20.f);
            }
            else 
            {
                updateEnergy(20.f);
                updateHealth(2.5f);
            }
        }
        else
        {
            if (foodAmmount > 3 && (energy < 20.f || health < 30.f))
            {
                foodAmmount -= 3;
                updateEnergy(60.f);
                updateHealth(20.f);
            }
            else if (foodAmmount > 0)
            {
                foodAmmount -= 1.f;
                updateEnergy(40.f);
                updateHealth(15.f);
            }
            else
            {
                updateEnergy(10.f);
                updateHealth(2.5f);
                gatherFood();
            }
        }
    }

    // Social Interactions

    void Pop::socialize()
    {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> event_result(0, 10000); 

        int result = event_result(rng);

        if (partner == 0 && result < 100 && age >= ADULT_AGE) 
        {
            auto& popsInCell = getCurrentCell().getPopsInCell();
            for (const auto& popID : popsInCell)
            {
                if (popID == id)
                    continue;

                if (globalPopulation.find(popID) == globalPopulation.end())
                    continue;

                auto& pop = globalPopulation.at(popID);
                if (pop->gender != this->gender && pop->partner == 0 && pop->age >= ADULT_AGE)
                {
                    this->partner = pop->id;
                    pop->partner = this->id;
                    addEventToHistory("Found a partner!, " + pop->getName());
                    pop->addEventToHistory("Found a partner!, " + this->getName());
                    break;
                }
            }
        }

        if (partner != 0 && result < 10) 
        {
            reproduce(); 
        }
    }

    void Pop::reproduce()
    {
        if (energy > 50.f && health > 50.f)
        {
            std::shared_ptr<Pop> child = std::make_shared<Pop>("John Doe", 0, position, civilization);

            auto& this_Partner = System_Utils::getPop(partner);

            addEventToHistory("Had a child with " + this_Partner->getName() + ", the child is: " + child->getName());
            this_Partner->addEventToHistory("Had a child with " + this->getName() + ", the child is: " + child->getName());

            this->children.push_back(child->id);
            this_Partner->children.push_back(child->id);

            if (gender == PopGender::Male)
            {
                child->parents.push_back(this->id);
                child->parents.push_back(this_Partner->id);
            }
            else
            {
                child->parents.push_back(this_Partner->id);
                child->parents.push_back(this->id);
            }

            globalPopulation.emplace(child->id, std::move(child));
            residence->updatePopulation(1);
        }
    }

    //RNG

    void Pop::event()
    {
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> event_dist(0, 5);

        int eventType = event_dist(rng);

        switch (eventType)
        {
        case 0:
            // Wound event
            updateHealth(-15.f);
            break;
        case 1:
            // Find extra food event
            updateFood(2);
            break;
        case 2:
            // Gain energy event
            updateEnergy(15.f);
            break;
        case 3:
            // Encounter wild animal event
            updateHealth(-20.f);
            updateEnergy(-10.f);
            break;
        case 4:
            // Discover a resource cache event
            updateFood(5);
            updateEnergy(10.f);
            break;
        case 5:
            //Wounded Severely Event
            updateHealth(-40.f);
            break;
        default:
            break;
        }
    }

    // Leader

    void Pop::manageCivilization()
    {
        auto civ = System_Utils::getCiv(civilization);
        auto& currentCell = getCurrentCell();

        uint32_t totalPossibleResources = currentCell.getMaxResources();
        uint32_t currentResources = currentCell.getFoodAmmount() + currentCell.getMaterialsAmmount();

        bool resourceScarcity = currentResources < totalPossibleResources * 0.3f;
        bool populationPressure = civ->getPopulation() / civ->getTerritory().size() > 10;

        if (resourceScarcity || populationPressure)
        {
            expandCivilization();
        }
    }

    void Pop::expandCivilization()
    {
        auto civ = System_Utils::getCiv(civilization);

        Vector2 newCellPos = lookForCell();
        auto &newCell = System_Utils::getCell(newCellPos);
        float cellCost = System_Utils::calculateCellCost(newCellPos);

        if (isCellMoveable(newCell) && newCell.getCurrentCivilization() == -1 && cellCost < civ->getMaterialsAmmount())
        {
            civ->expand(newCellPos);
            civ->incrementMaterials((uint32_t)-cellCost);
        }
    }

    void Pop::simulate()
    {
        if (this == nullptr)
            return;
            
        updateEnergy(-10.f); // Daily Energy Loss

        if (System::Time::getCurrentTime() - lastAgeUpdateTime >= static_cast<int>(orbitalPeriod))
        {
            this->lastAgeUpdateTime = System::Time::getCurrentTime();
            ++age;
            if (age > 50)
            {
                --max_health;
                updateHealth(0.0f); // clamps it to the max health
            }
        }

        if (energy < 50.f || health < max_health * 0.75f)
        {
            rest();
            return; // skip the day for resting
        } 
        else
        {
            socialize();
        }

        if (isLookingForNewResidence && !isLeader)
        {
            if (!isNewCellAdequate)
            {
                move(lookForCell());
            }
            else
            {
                migrate(position);
            }
        }

        if (isLeader)
            manageCivilization();

        if (energy < 50.0f || foodAmmount < 2)
        {
            gatherFood();
        }

        if (!((residence->getTemperature() >= 0.3 && residence->getTemperature() <= 0.9) && residence->getHumidity() <= 0.8))
        {
            health -= 5.f;
            isLookingForNewResidence = true;
        }

        handleDeath();
    }
} // namespace Simulation_AI
