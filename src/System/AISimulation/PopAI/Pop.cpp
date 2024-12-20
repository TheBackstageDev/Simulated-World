#include "../src/Headers/System/AISimulation/PopAI/Pop.hpp"
#include "../src/Headers/Utils/WorldUtils.hpp"

#include <cmath>
#include <random>

namespace Simulation_AI
{
    static uint32_t lastID{0};
    Pop::Pop(std::string name, char32_t age, Vector2 position, uint32_t civilization)
    : name(name), age(age), id(++lastID), position(position), civilization(civilization), birthDate(System::Time::getCurrentTime())
    {
        residence = &System_Utils::getCell(position);
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

        uint32_t ammountTaxed = floorf(increment * tax);
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
        auto& currentCell = System_Utils::getCell(cell);
        position = cell;

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
        std::uniform_int_distribution<int> delta_reources(0, 2);

        std::uniform_int_distribution<int> event_happen(1, 10);

        if (event_happen(rng) == 1)
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
                updateHealth(15.f);
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
                updateEnergy(50.f);
                updateHealth(15.f);
            }
            else if (foodAmmount > 0)
            {
                foodAmmount -= 1.f;
                updateEnergy(25.f);
                updateHealth(10.f);
            }
            else
            {
                updateEnergy(10.f);
                updateHealth(2.5f);
                gatherFood();
            }
        }
    }

    //RNG

    void Pop::event()
    {
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> event_dist(0, 4);

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
        default:
            break;
        }
    }
    // Leader

    void Pop::manageCivilization()
    {
        auto civ = System_Utils::getCiv(civilization);
        auto& currentCell = System_Utils::getCell(position);

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

        if (isCellMoveable(newCell) && newCell.getCurrentCivilization() == -1)
        {
            civ->expand(newCellPos);
        }
    }

    void Pop::simulate()
    {
        auto& currentCell = System_Utils::getCell(position);

        updateEnergy(-10.f); // Daily Energy Loss

        if (energy < 50.f || health < max_health * 0.75f)
        {
            rest();
            return; // skip the day for resting
        }

        if (isLookingForNewResidence && !isLeader)
        {
            if (!isNewCellAdequate)
            {
                Vector2 cell = lookForCell();
                move(cell);
            }
            else
            {
                migrate(position);
            }
        }

        if (isLeader)
            manageCivilization();

        if (System::Time::getCurrentTime() - lastAgeUpdateTime >= static_cast<int>(orbitalPeriod))
        {
            lastAgeUpdateTime = System::Time::getCurrentTime();
            ++age;
            if (age > 50)
            {
                --max_health;
                updateHealth(0.0f); // clamps it to the max health
            }
        }

        if (!((residence->getTemperature() >= 0.3 && residence->getTemperature() <= 0.9) && residence->getHumidity() <= 0.8))
        {
            health -= 0.5f;
            isLookingForNewResidence = true;
        }

        if (energy < 50.0f || foodAmmount < 2)
        {
            gatherFood();
        }
    }
} // namespace Simulation_AI
