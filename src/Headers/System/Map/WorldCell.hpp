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

        void determineBiome();
        Color interpolateColor() const;

    public:
        GridCell(std::string name, float elevation, float temperature, float humidity);
        GridCell() : name(""), elevation(0.0f) {}
        ~GridCell();

        void setName(std::string newName) { this->name = newName; }
        std::string& getName() { return this->name; }
        uint32_t getID() { return id; }
        float getElevation() { return this->elevation; }
        float getTemperature() { return this->temperature; } 
        Color getTemperatureColor() const;
        float getHumidity() { return this->humidity;}
        Color getHumidityColor() const;
        Biome getBiome() const;
        void setBiome(Biome newBiome) { this->biome = newBiome; }
        Color getBiomeColor() const;

        Color getColor() const;
    };
} // namespace World
