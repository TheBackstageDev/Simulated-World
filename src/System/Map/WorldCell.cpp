#include "../src/Headers/System/Map/WorldCell.hpp"
#include "../src/Headers/Utils/WorldDefinitions.hpp"

namespace World
{
    uint32_t GridCell::lastID{0};

    GridCell::GridCell(std::string name, float elevation, float temperature, float humidity)
        : name(name), elevation(elevation), temperature(temperature), humidity(humidity)
    {
        this->id = ++lastID;
        determineBiome(); 
    }

    GridCell::~GridCell() {}

    void GridCell::determineBiome()
    {
        if (elevation < SEA_LEVEL)
        {
            if (temperature < 0.3)
            {
                biome = Biome::Arctic;
            }
            else 
            {
                biome = Biome::Ocean;
            }
        }
        else if (elevation < 0.1 && humidity > 0.5)
        {
            biome = Biome::Beach;
        }
        else if (elevation > 0.8f)
        {
            biome = Biome::Mountain;
        }
        else if (temperature < 0.2f)
        {
            biome = Biome::Tundra;
        }
        else if (humidity < 0.3f)
        {
            biome = Biome::Desert;
        }
        else if (humidity > 0.6f)
        {
            biome = Biome::Forest;
        }
        else if (temperature > 0.5f && humidity < 0.4f)
        {
            biome = Biome::Savanna;
        }
        else if (temperature < 0.3)
        {
            biome = Biome::Arctic;
        }
        else
        {
            biome = Biome::Grassland;
        }
    }

    Biome GridCell::getBiome() const
    {
        return biome;
    }

    Color GridCell::interpolateColor() const
    {
        switch (biome)
        {
        case Biome::Ocean:
            return {0, 0, 255, 255}; // Blue
        case Biome::Beach:
            return {255, 255, 102, 255}; // Light Yellow
        case Biome::Forest:
            return {34, 139, 34, 255}; // Forest Green
        case Biome::Desert:
            return {237, 201, 175, 255}; // Sand
        case Biome::Mountain:
            return {255, 255, 255, 255}; // Gray
        case Biome::Tundra:
            return {192, 192, 192, 255}; // Light Gray
        case Biome::Grassland:
            return {124, 252, 0, 255}; // Lawn Green
        case Biome::Savanna:
            return {189, 183, 107, 255}; // Dark Khaki
        case Biome::Arctic:
            return {173, 216, 230, 255}; // Light Blue (Icy color)
        default:
            return {255, 255, 255, 255}; // Default White
        }
    }

    Color GridCell::getColor() const
    {
        return interpolateColor();
    }

} // namespace World
