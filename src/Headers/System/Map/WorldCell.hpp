#pragma once

#include <raylib.h>
#include <string>

namespace World
{
    class GridCell
    {
    private:
        std::string name;
        float elevation;
        float temperature;
        float humidity;
    public:
        GridCell(std::string name, float elevation, float temperature, float humidity);
        GridCell() : name(""), elevation(0.0f) {}
        ~GridCell();

        void setName(std::string newName) { this->name = newName; }
        std::string& getName() { return this->name; }
        float getElevation() { return this->elevation; }
        float getTemperature() { return this->temperature; } 
        float getHumidity() { return this->humidity;}

        Color getColor() const;
    };
} // namespace World
