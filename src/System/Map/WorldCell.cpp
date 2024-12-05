#include "../src/Headers/System/Map/WorldCell.hpp"

namespace World
{
    GridCell::GridCell(std::string name, float elevation) : name(name), elevation(elevation)
    {
    }

    GridCell::~GridCell() {}
} // namespace World
