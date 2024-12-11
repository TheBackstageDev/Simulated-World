#pragma once

namespace World
{
    class Planet
    {
    public:
        Planet();
        Planet(float axialTilt, float orbitalEccentricity, float distanceFromStar, float orbitalPeriod, float rotationPeriod);

        // Accessor methods
        float getAxialTilt() const;
        float getOrbitalEccentricity() const;
        float getDistanceFromStar() const;
        float getOrbitalPeriod() const;
        float getRotationPeriod() const;

        // Update methods
        void updateTemperature();
        void updateHumidity();
    private:
        float axialTilt;           // In degrees
        float orbitalEccentricity; // Orbital eccentricity (0 = circular, 1 = parabolic)
        float distanceFromStar;    // Average distance from star in AU
        float orbitalPeriod;       // Orbital period in days
        float rotationPeriod;      // Rotation period in hours

        float calculateSolarEnergy(int latitude) const;
    };
} // namespace World
