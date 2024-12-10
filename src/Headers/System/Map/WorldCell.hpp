#pragma once

#include <raylib.h>
#include <string>

namespace World
{
    enum class Biome
    {
        Ocean,
        Beach,
        Forest,
        Desert,
        Mountain,
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
        float getHumidity() { return this->humidity;}
        Biome getBiome() const;

        Color getColor() const;
    };
} // namespace World
