#include "../src/Headers/System/Map/WorldCell.hpp"
#include "../src/Headers/System/Time.hpp"
#include "../src/Headers/Utils/WorldDefinitions.hpp"

namespace World
{
    uint32_t GridCell::lastID{0};

    GridCell::GridCell(std::string name, Vector2 position, float elevation, float temperature, float humidity)
        : name(name), elevation(elevation), temperature(temperature), humidity(humidity), position(position)
    {
        this->id = ++lastID;
        determineBiome();
        determineResourceAmmount();
    }

    GridCell::~GridCell() {}

    void GridCell::determineBiome()
    {
        if (elevation < SEA_LEVEL)
        {
            if (temperature < ARCTIC_TEMP)
            {
                biome = Biome::Arctic;
            }
            else if (temperature >= 1.0f)
            {
                biome = Biome::Dirt;
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
        else if (temperature > 0.3f && humidity < 0.3f)
        {
            biome = Biome::Desert;
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
        else if (temperature < 0.2f && humidity < 0.3f)
        {
            biome = Biome::Tundra;
        }
        else
        {
            biome = Biome::Grassland;
        }
    }

    Color GridCell::interpolateColor() 
    {
         if ((this->biome == Biome::RainForest) && this->cellColor.a != 0)
            return cellColor;

        Color color;
        switch (biome)
        {
        case Biome::Ocean:
            color = {0, 0, 240, 255}; // Blue
            break;
        case Biome::River:
            color = {30, 144, 255, 255}; // Dodger Blue
            break;
        case Biome::Dirt:
            color = {155, 118, 83, 255}; // Dirt Brown
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
            color = {237, 201, 150, 255}; // Sand
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
            color = {70, 179, 70, 255}; // Lawn Green
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
            if (elevation > 1.0f && temperature < 1.0f)
                adjustment = elevation * 0.75f - temperature * 0.2f;

            color.r = static_cast<unsigned char>(std::min(255.0f, color.r  * adjustment));
            color.g = static_cast<unsigned char>(std::min(255.0f, color.g  * adjustment));
            color.b = static_cast<unsigned char>(std::min(255.0f, color.b  * adjustment));
        }

        if (biome == Biome::Ocean || biome == Biome::Hill || biome == Biome::Dirt)
        {
            if (biome == Biome::Ocean && elevation > -0.1f)
            {
                adjustment = (elevation + 1.0f) * 2.0f; 
            }
            if (biome == Biome::Ocean || biome == Biome::Dirt)
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
        if (biome == Biome::RainForest || biome == Biome::Forest)
        {
            adjustment = (1 - humidity) + (1 - temperature * 2);

            if (biome == Biome::RainForest)
                adjustment /= 2;

            color.r = static_cast<unsigned char>(std::min(225.0f, color.r + color.r * adjustment));
            color.g = static_cast<unsigned char>(std::min(225.0f, color.g + color.g * adjustment));
            color.b = static_cast<unsigned char>(std::min(225.0f, color.b + color.b * adjustment));
        }

        if (biome == Biome::Desert)
        {
            adjustment = (1 - humidity * 2) + (temperature * 2);

            color.r = static_cast<unsigned char>(std::min(210.0f, color.r * adjustment));
            color.g = static_cast<unsigned char>(std::min(210.0f, color.g * adjustment));
            color.b = static_cast<unsigned char>(std::min(210.0f, color.b * adjustment));
        }

        if (biome == Biome::Grassland)
        {
            adjustment = ((1 - humidity) + (1 - temperature)) * 1.2f;
 
            color.r = static_cast<unsigned char>(std::min(225.0f, color.r * adjustment));
            color.g = static_cast<unsigned char>(std::min(225.0f, color.g * adjustment));
            color.b = static_cast<unsigned char>(std::min(225.0f, color.b * adjustment));
        }

        cellColor = color;
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
        case Biome::Dirt:
            color = {155,118,83, 255}; // Dirt Brown
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
        if (temperature <= 0.1f)
        {
            color = {0, 0, 255, 255}; // Very Cold (Blue)
        }
        else if (temperature <= 0.3f)
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

    Color GridCell::getPopulationColor() const
    {
        const float density = static_cast<float>(population) / Cellsize;

        Color color;

        if (density == 0.f)
        {
            color = {100, 100, 100, 255}; // Gray
        }
        else if (density < 100.0f)
        {
            color = {0, 0, 255, 255}; // Blue
        }
        else if (density < 500.0f)
        {
            color = {0, 128, 255, 255}; // Light Blue
        }
        else if (density < 1000.0f)
        {
            color = {0, 255, 0, 255}; // Green
        }
        else if (density < 2000.0f)
        {
            color = {173, 255, 47, 255}; // YellowGreen
        }
        else if (density < 3000.0f)
        {
            color = {255, 255, 0, 255}; // Yellow
        }
        else if (density < 4000.0f)
        {
            color = {255, 165, 0, 255}; // Orange
        }
        else
        {
            color = {255, 0, 0, 255}; // Red
        }

        return color;
    }

    Color GridCell::getColor()
    {
        return interpolateColor();
    }

    void GridCell::updateTemperature(float newTemp)
    {
        this->temperature = newTemp;

        const float ARCTIC_TEMP_UPPER = ARCTIC_TEMP + 0.02f; 
        const float ARCTIC_TEMP_LOWER = ARCTIC_TEMP - 0.02f;

        if (this->biome == Biome::Arctic && temperature > ARCTIC_TEMP_UPPER)
        {
            setBiome(Biome::Ocean);
        }
        if (this->biome == Biome::Ocean && temperature < ARCTIC_TEMP_LOWER)
        {
            setBiome(Biome::Arctic);
        }
        if (this->biome == Biome::Ocean && temperature >= 1.0f)
        {
            setBiome(Biome::Dirt);
        }
        if (this->biome == Biome::Dirt && temperature < 1.0f)
        {
            setBiome(Biome::Ocean);
        }
    }

    void GridCell::determineResourceAmmount()
    {
        switch (biome)
        {
        case Biome::Ocean:
            MaxResourcesAmmount = 30; 
            break;
        case Biome::River:
            MaxResourcesAmmount = 70; 
            break;
        case Biome::Beach:
            MaxResourcesAmmount = 30; 
            break;
        case Biome::Forest:
            MaxResourcesAmmount = 100; 
            break;
        case Biome::RainForest:
            MaxResourcesAmmount = 120; 
            break;
        case Biome::Desert:
            MaxResourcesAmmount = 5; 
            break;
        case Biome::Mountain:
            MaxResourcesAmmount = 60; 
            break;
        case Biome::Hill:
            MaxResourcesAmmount = 80; 
            break;
        case Biome::Tundra:
            MaxResourcesAmmount = 20; 
            break;
        case Biome::Grassland:
            MaxResourcesAmmount = 90; 
            break;
        case Biome::Savanna:
            MaxResourcesAmmount = 60; 
            break;
        default:
            MaxResourcesAmmount = 10; 
            break;
        }

        float elevationModifier = 1.0f;
        if (elevation > SEA_LEVEL)
        {
            elevationModifier = 1.0f - (elevation * 0.1f);
        }

        MaxResourcesAmmount = 100 * static_cast<uint32_t>(MaxResourcesAmmount * elevationModifier * (0.9f + (static_cast<float>(rand()) / RAND_MAX) * 0.2f));
        MaterialsAmmount = floorf(0.75 * MaxResourcesAmmount);

        FoodAmmount = floorf(0.25 * MaxResourcesAmmount);

        switch (biome)
        {
        case Biome::Hill:
        case Biome::Mountain:
            FoodAmmount *= 0.05f;
            break;
        case Biome::Desert:
            FoodAmmount *= 0.05f;
            break;
        default:
            break;
        }
    }

    std::vector<Vector2> GridCell::getNeighbours()
    {
        std::vector<Vector2> neighbours;
        neighbours.push_back({position.x - 1, position.y});
        neighbours.push_back({position.x + 1, position.y});
        neighbours.push_back({position.x, position.y - 1});
        neighbours.push_back({position.x, position.y + 1});
        neighbours.push_back({position.x - 1, position.y - 1});
        neighbours.push_back({position.x + 1, position.y - 1});
        neighbours.push_back({position.x - 1, position.y + 1});
        neighbours.push_back({position.x + 1, position.y + 1});

        return neighbours;
    }
} // namespace World
