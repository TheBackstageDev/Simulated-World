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

    Vector2 Pop::lookForCell()
    {
        std::mt19937 rng(std::random_device{}());
        std::mt19937 rng2(std::random_device{}());

        std::uniform_int_distribution<int> offset_dist(-1, 1);
        int randXoffset = offset_dist(rng);
        int randYoffset = offset_dist(rng2);

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

    void Pop::isCellAdequate(GridCell &cell)
    {
        // Check if the cell is not the current residence
        if (isCellMoveable(cell))
        {
            bool hasMoreFood = cell.getFoodAmmount() > residence->getFoodAmmount();
            bool optimalTemperature = cell.getTemperature() >= 0.3 && cell.getTemperature() <= 0.9;
            bool optimalHumidity = cell.getHumidity() <= 0.8;

            if (hasMoreFood || optimalTemperature && optimalHumidity)
                isNewCellAdequate = true;
        }
    }

    // AI Behaviour

    void Pop::move(Vector2 cell)
    {
        auto& currentCell = System_Utils::getCell(cell);
        position = cell;

        isCellAdequate(currentCell);
    }

    void Pop::migrate(Vector2 newCell)
    {
        if (residence != nullptr)
            residence->updatePopulation(-1);

        residence = &System_Utils::getCell(newCell);
        position = newCell;
        residence->updatePopulation(+1);

        //Resets Flags
        isNewCellAdequate = false;
        isLookingForNewResidence = false;
    }

    // Leader

    void Pop::manageCivilization()
    {

    }

    void Pop::simulate()
    {
        auto& currentCell = System_Utils::getCell(position);

        if (isLookingForNewResidence)
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

        static uint32_t lastAgeUpdateTime{0};

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

        if (currentCell.getTemperature() < 0.3 || currentCell.getTemperature() > 0.9 || currentCell.getHumidity() > 0.8)
        {
            health -= 0.5f;

            if (!isLeader) 
                isLookingForNewResidence = true;
        }
    }
} // namespace Simulation_AI
