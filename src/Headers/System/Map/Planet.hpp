#pragma once

#include <raygui.h>

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

        float calculateSolarEnergy(int latitude) const;
    public:
        Planet();
        Planet(float axialTilt, float orbitalEccentricity, float distanceFromStar, float orbitalPeriod, float rotationPeriod);

        // Accessor methods
        float getAxialTilt() const { return axialTilt; }
        float getOrbitalEccentricity() const { return orbitalEccentricity; }
        float getDistanceFromStar() const { return distanceFromStar; }
        float getOrbitalPeriod() const { return orbitalPeriod; }
        float getRotationPeriod() const { return rotationPeriod; }

        // Update methods
        void updateTemperature();
        void updateHumidity();

        // Other
        static void drawPlanetDepiction(Vector2 posCenter, float radius, float axialTilt);
        static void drawPlanetOrbitDepiction(Vector2 posCenter, float radius, float orbitalEccentricity, float distanceFromStar);
    };
} // namespace World
