#pragma once

#include <string>

namespace World
{
    class GridCell
    {
    private:
        std::string name;
        float elevation;
    public:
        GridCell(std::string name, float elevation);
        GridCell() : name(""), elevation(0.0f) {}
        ~GridCell();

        void setName(std::string newName) { this->name = newName; }
        std::string& getName() { return this->name; }
        float getElevation() { return this->elevation; }
    };
} // namespace World
