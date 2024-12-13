#pragma once

#include <raygui.h>
#include "../src/Headers/System/Map/WorldMap.hpp"

namespace World
{
    class Planet
    {
    private:
        float axialTilt;           // In degrees
        float orbitalEccentricity; // Orbital eccentricity (0 = circular, 1 = parabolic)
        float distanceFromStar;    // Average distance from star in AU
        float orbitalPeriod;       // Orbital period in days
        float rotationPeriod;      // Rotation period in hours

        float currentOrbitalSpeed; // Speed at current point in Orbit in Km/s
        static float percentInOrbit; // Percentage of the orbit it has completed

        void updateOrbitalParams();
        float calculateSolarEnergy(int latitude) const;
    public:
        Planet();
        Planet(float axialTilt, float orbitalEccentricity, float distanceFromStar, float rotationPeriod);

        // Accessor methods
        float getAxialTilt() const { return axialTilt; }
        float getOrbitalEccentricity() const { return orbitalEccentricity; }
        float getDistanceFromStar() const { return distanceFromStar; }
        float getOrbitalPeriod() const { return orbitalPeriod; }
        float getRotationPeriod() const { return rotationPeriod; }

        // Update methods
        void updatePlanet();

        // Other
        static void drawPlanetDepiction(Vector2 posCenter, float radius, float axialTilt);
        static void drawPlanetOrbitDepiction(Vector2 posCenter, float radius, float orbitalEccentricity, float distanceFromStar);
        void drawPlanetOrbit(Vector2 posCenter, float radius);
    };
} // namespace World
