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
            if (temperature < 0.25f)
            {
                biome = Biome::Arctic;
            }
            else 
            {
                biome = Biome::Ocean;
            }
        }
        else if (elevation < 0.1f && humidity > 0.5f)
        {
            biome = Biome::Beach;
        }
        else if (elevation > 0.6f)
        {
            if (elevation > 0.8f)
            {
                biome = Biome::Mountain;
            }
            else 
            {
                biome = Biome::Hill;
            }
        }
        else if (temperature < 0.2)
        {
            biome = Biome::Arctic;
        }
        else if (humidity < 0.3f)
        {
            biome = Biome::Desert;
        }
        else if (temperature < 0.3f)
        {
            biome = Biome::Tundra;
        }
        else if (humidity > 0.65f)
        {
            biome = Biome::RainForest;
        }
        else if (humidity > 0.5f)
        {
            biome = Biome::Forest;
        }
        else if (temperature > 0.45f && humidity < 0.4f)
        {
            biome = Biome::Savanna;
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
        Color color;
        switch (biome)
        {
        case Biome::Ocean:
            color = {0, 0, 240, 255}; // Blue
            break;
        case Biome::River:
            color = {30, 144, 255, 255}; // Dodger Blue
            break;
        case Biome::Beach:
            color = {255, 255, 102, 255}; // Light Yellow
            break;
        case Biome::Forest:
            color = {34, 139, 34, 255}; // Forest Green
            break;
        case Biome::RainForest:
            color = {0, 100, 0, 255}; // Dark Green
            break;
        case Biome::Desert:
            color = {237, 201, 175, 255}; // Sand
            break;
        case Biome::Mountain:
            color = {255, 255, 255, 255}; // White
            break;
        case Biome::Hill:
            color = {122, 122, 122, 255}; // Gray
            break;
        case Biome::Tundra:
            color = {192, 192, 210, 255}; // Light Gray
            break;
        case Biome::Grassland:
            color = {124, 252, 0, 255}; // Lawn Green
            break;
        case Biome::Savanna:
            color = {189, 183, 107, 255}; // Dark Khaki
            break;
        case Biome::Arctic:
            color = {173, 216, 230, 255}; // Light Blue (Icy color)
            break;
        default:
            color = {0, 0, 0, 255}; // Default Black
            break;
        }

        //Elevation Based Interpolations
        
        float adjustment = elevation * 0.3f;

        if (biome == Biome::Hill)
            adjustment = (1 - elevation) * 0.7f;

        if (biome == Biome::Mountain)
        {
            if (elevation > 1.0f)
                adjustment = elevation * 0.85f;

            color.r = static_cast<unsigned char>(std::min(255.0f, color.r  * adjustment));
            color.g = static_cast<unsigned char>(std::min(255.0f, color.g  * adjustment));
            color.b = static_cast<unsigned char>(std::min(255.0f, color.b  * adjustment));
        }

        if (biome == Biome::Ocean || biome == Biome::Hill)
        {
            if (biome == Biome::Ocean && elevation > -0.1f)
            {
                adjustment = (elevation + 1.0f) * 2.0f; 
            }
            if (biome == Biome::Ocean)
            {
                color.r = static_cast<unsigned char>(std::min(255.0f, color.r + color.r * adjustment * 0.5f)); 
                color.g = static_cast<unsigned char>(std::min(255.0f, color.g + color.g * adjustment * 2.f));
                color.b = static_cast<unsigned char>(std::min(255.0f, color.b + color.b * adjustment * 1.5f)); 
            }
            else 
            {
                color.r = static_cast<unsigned char>(std::min(255.0f, color.r + color.r * adjustment));
                color.g = static_cast<unsigned char>(std::min(255.0f, color.g + color.g * adjustment));
                color.b = static_cast<unsigned char>(std::min(255.0f, color.b + color.b * adjustment));
            }
        }

        //Humidity and Temperature Based Interpolations

        return color;
    }

    Color GridCell::getBiomeColor() const
    {
        Color color;
        switch (biome)
        {
        case Biome::Ocean:
            color = {0, 0, 255, 255}; // Blue
            break;
        case Biome::River:
            color = {30, 144, 255, 255}; // Dodger Blue
            break;
        case Biome::Beach:
            color = {255, 255, 102, 255}; // Light Yellow
            break;
        case Biome::Forest:
            color = {34, 139, 34, 255}; // Forest Green
            break;
        case Biome::RainForest:
            color = {0, 100, 0, 255}; // Dark Green
            break;
        case Biome::Desert:
            color = {237, 201, 175, 255}; // Sand
            break;
        case Biome::Mountain:
            color = {255, 255, 255, 255}; // White
            break;
        case Biome::Hill:
            color = {155, 155, 155, 255}; // Gray
            break;
        case Biome::Tundra:
            color = {192, 192, 210, 255}; // Light Gray
            break;
        case Biome::Grassland:
            color = {124, 252, 0, 255}; // Lawn Green
            break;
        case Biome::Savanna:
            color = {189, 183, 107, 255}; // Dark Khaki
            break;
        case Biome::Arctic:
            color = {173, 216, 230, 255}; // Light Blue (Icy color)
            break;
        default:
            color = {0, 0, 0, 255}; // Default Black
            break;
        }

        return color;
    }

    Color GridCell::getHumidityColor() const
    {
        Color color;
        if (humidity <= 0.2f)
        {
            color = {255, 228, 181, 255}; // Very Dry (Moccasin)
        }
        else if (humidity <= 0.4f)
        {
            color = {237, 201, 175, 255}; // Dry (Sandy)
        }
        else if (humidity <= 0.6f)
        {
            color = {144, 238, 144, 255}; // Moderate (Light Green)
        }
        else if (humidity <= 0.8f)
        {
            color = {34, 139, 34, 255}; // Humid (Forest Green)
        }
        else if (humidity <= 0.9f)
        {
            color = {0, 100, 0, 255}; // Very Humid (Dark Green)
        }
        else
        {
            color = {0, 80, 0, 255}; // Extremely Humid (Darker Green)
        }

        return color;
    }

    Color GridCell::getTemperatureColor() const
    {
        Color color;
        if (temperature <= 0.2f)
        {
            color = {0, 0, 255, 255}; // Very Cold (Blue)
        }
        else if (temperature <= 0.4f)
        {
            color = {135, 206, 235, 255}; // Cold (Sky Blue)
        }
        else if (temperature <= 0.6f)
        {
            color = {255, 255, 0, 255}; // Moderate (Yellow)
        }
        else if (temperature <= 0.8f)
        {
            color = {255, 165, 0, 255}; // Warm (Orange)
        }
        else
        {
            color = {255, 0, 0, 255}; // Hot (Red)
        }
        return color;
    }

    Color GridCell::getColor() const
    {
        return interpolateColor();
    }

} // namespace World
