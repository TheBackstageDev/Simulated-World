#include "../src/Headers/Utils/WorldDefinitions.hpp"

namespace World
{
    std::string World::biomeToString(Biome biome)
    {
        switch (biome)
        {
        case Biome::Ocean:
            return "Ocean";
        case Biome::Beach:
            return "Beach";
        case Biome::Forest:
            return "Forest";
        case Biome::Desert:
            return "Desert";
        case Biome::Mountain:
            return "Mountain";
        case Biome::Hill:
            return "Hill";
        case Biome::Tundra:
            return "Tundra";
        case Biome::Grassland:
            return "Grassland";
        case Biome::Savanna:
            return "Savanna";
        case Biome::Arctic:
            return "Arctic";
        default:
            return "Unknown Biome";
        }
    }
} // namespace World
