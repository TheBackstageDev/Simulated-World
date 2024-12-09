#include "../src/Headers/System/Map/WorldCell.hpp"

namespace World
{
    GridCell::GridCell(std::string name, float elevation, float temperature, float humidity) 
    : name(name), elevation(elevation), temperature(temperature), humidity(humidity)
    {
    }

    GridCell::~GridCell() {}

    Color World::GridCell::getColor() const
    {
        unsigned char colorValue = static_cast<unsigned char>(elevation * 255.0f);
        return {colorValue, colorValue, colorValue, 255};
    }

} // namespace World
