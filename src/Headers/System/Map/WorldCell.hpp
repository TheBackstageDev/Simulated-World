#pragma once

#include <raylib.h>
#include <string>

namespace World
{
    enum class Biome
    {
        Ocean,
        River,
        Beach,
        Forest,
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

        Vector2 position;

        void determineBiome();
        Color interpolateColor() const;

    public:
        GridCell(std::string name, Vector2 position, float elevation, float temperature, float humidity);
        GridCell() : name(""), elevation(0.0f), position({1.0f, 1.0f}) {}
        ~GridCell();

        //Setters
        void setName(std::string newName) { this->name = newName; }
        void updateElevation(float increment) { this->elevation += increment; }

        //Getters
        std::string& getName() { return this->name; }
        uint32_t getID() { return id; }
        float getElevation() const { return this->elevation; }
        float getTemperature() const { return this->temperature; } 
        Color getTemperatureColor() const;
        float getHumidity() const { return this->humidity;}
        Color getHumidityColor() const;
        Biome getBiome() const;
        void setBiome(Biome newBiome) { this->biome = newBiome; }
        Color getBiomeColor() const;
        Vector2 getPos() const { return position; }
        Color getColor() const;
    };

    struct CompareGridCell
    {
        bool operator()(const GridCell &lhs, const GridCell &rhs) const
        {
            return lhs.getElevation() < rhs.getElevation();
        }
    };
} // namespace World
