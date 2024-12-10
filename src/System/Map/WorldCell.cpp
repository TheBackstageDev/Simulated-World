#include "../src/Headers/System/Map/WorldCell.hpp"

namespace World
{
    uint32_t World::GridCell::lastID{0};
    GridCell::GridCell(std::string name, float elevation, float temperature, float humidity) 
    : name(name), elevation(elevation), temperature(temperature), humidity(humidity)
    {
        this->id = ++lastID;
    }

    GridCell::~GridCell() {}

    Color World::GridCell::getColor() const
    {
        unsigned char colorValue = static_cast<unsigned char>(elevation * 255.0f);
        return {colorValue, colorValue, colorValue, 255};
    }

} // namespace World
