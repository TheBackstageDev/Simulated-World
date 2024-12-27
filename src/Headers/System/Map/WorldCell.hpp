#pragma once

#include <raylib.h>
#include <string>
#include <raymath.h>
#include <set>
#include <vector>

namespace World
{
    enum class Biome
    {
        Ocean,
        River,
        Beach,
        Forest,
        Dirt,
        RainForest,
        Desert,
        Mountain,
        Hill,
        Tundra,
        Grassland,
        Savanna,
        Arctic,
    };

    class GridCell
    {
    private:
        static uint32_t lastID;
        uint32_t id;
        std::string name;
        float elevation;
        float temperature;
        float humidity;
        Biome biome;

        int32_t civilization{-1}; // the civilization that owns this cell

        uint32_t MaxResourcesAmmount;
        uint32_t MaterialsAmmount;
        uint32_t FoodAmmount;
        uint32_t population{0};

        Vector2 position;
        Color cellColor{0, 0, 0, 0};

        void determineBiome();
        void determineResourceAmmount();
        Color interpolateColor();

        std::set<uint32_t> popsInCell;

    public:
        GridCell(std::string name, Vector2 position, float elevation, float temperature, float humidity);
        GridCell() : name(""), elevation(0.0f), position({1.0f, 1.0f}) {}
        ~GridCell();

        //Setters
        void setName(std::string newName) { this->name = newName; }
        void updateElevation(float increment) { this->elevation += increment; }
        void updateTemperature(float newTemp);
        void updateHumidity(float increment) { this->humidity = increment; }
        void updateFood(uint32_t increment)
        {
            this->FoodAmmount += increment;
            this->FoodAmmount = (uint32_t)Clamp((float)FoodAmmount, 0u, static_cast<uint32_t>(floorf((float)MaxResourcesAmmount * 0.25f)));
        }
        void movePop(uint32_t id, bool isMovingIn)
        {
            if (isMovingIn)
            {
                popsInCell.insert(id);
            }
            else
            {
                popsInCell.erase(id);
            }
        }
        void updateMaterials(uint32_t increment)
        {
            this->MaterialsAmmount += increment;
            this->MaterialsAmmount = (uint32_t)Clamp((float)MaterialsAmmount, 0u, static_cast<uint32_t>(floorf((float)MaxResourcesAmmount * 0.75f)));
        }
        void setCivilizationOwnership(uint32_t id) { civilization = id; }
        void updatePopulation(uint32_t increment) { population += increment; population = (uint32_t)Clamp((float)population, 0u, sizeof(uint32_t)); }
        void setBiome(Biome newBiome) { this->biome = newBiome; }

        //Getters
        std::string& getName() { return this->name; }
        uint32_t getID() const { return id; }
        uint32_t getMaterialsAmmount() const { return MaterialsAmmount; }
        uint32_t getFoodAmmount() const { return FoodAmmount; }
        uint32_t getMaxResources() const { return MaxResourcesAmmount; }
        uint32_t getMaxFoodAmmount() const { return static_cast<uint32_t>(MaxResourcesAmmount * .25f); }
        uint32_t getMaxMaterialsAmmount() const { return static_cast<uint32_t>(MaxResourcesAmmount * .75f); }
        uint32_t getPopulation() const { return population; }
        int32_t getCurrentCivilization() { return civilization; }
        float getElevation() const { return this->elevation; }
        float getTemperature() const { return this->temperature; } 
        float getHumidity() const { return this->humidity; }
        Color getPopulationColor() const;
        Color getHumidityColor() const;
        Color getTemperatureColor() const;
        Color getBiomeColor() const;
        Color getColor();
        std::set<uint32_t>& getPopsInCell() { return popsInCell; }
        Vector2 getPos() const { return position; }
        std::vector<Vector2> getNeighbours();
        Biome getBiome() const { return this->biome; }
    };

    struct CompareGridCell
    {
        bool operator()(const GridCell &lhs, const GridCell &rhs) const
        {
            return lhs.getElevation() < rhs.getElevation();
        }
    };
} // namespace World
