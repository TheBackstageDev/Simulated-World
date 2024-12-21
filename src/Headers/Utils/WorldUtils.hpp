#pragma once

#include "../src/Headers/System/Map/WorldGeneration.hpp"

using namespace World;

namespace System_Utils
{
    std::string biomeToString(Biome biome);
    bool isPeakDistantFromOthers(std::priority_queue<GridCell, std::vector<GridCell>, CompareGridCell> mountainPeaks, Vector2 peak);
    GridCell &getCell(Vector2 cell);
    float calculateCellCost(Vector2 cell);

    Simulation_AI::Civilization* getCiv(uint32_t civID);
    std::shared_ptr<Simulation_AI::Pop> getPop(uint32_t popID);
} // namespace System_Utils
