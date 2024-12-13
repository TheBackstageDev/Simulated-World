#include "../src/Headers/System/Map/Planet.hpp"
#include "../src/Headers/Utils/WorldDefinitions.hpp"
#include <raymath.h>

namespace World
{
    float Planet::percentInOrbit{0.0f};
    Planet::Planet(float axialTilt, float orbitalEccentricity, float distanceFromStar, float rotationPeriod)
    : axialTilt(axialTilt), orbitalEccentricity(orbitalEccentricity), distanceFromStar(distanceFromStar), rotationPeriod(rotationPeriod)
    {
        double semiMajorAxisMeters = distanceFromStar * 1.49e11; // Convert AU to meters
        orbitalPeriod = sqrt((4 * powf(PI, 2) * semiMajorAxisMeters * semiMajorAxisMeters * semiMajorAxisMeters) / (GRAVITATIONAL_CONSTANT * STAR_MASS)) / (60 * 60 * 24);
    }

    Planet::Planet()
    : axialTilt(23.4f), orbitalEccentricity(0.01f), distanceFromStar(1.0f), orbitalPeriod(365.f), rotationPeriod(24.f)
    {}

    //Plnaet Functions

    void Planet::updateOrbitalParams()
    {
        float orbitalSpeed = 1.0f / orbitalPeriod;
        percentInOrbit += orbitalSpeed;
        if (percentInOrbit >= 1.0f)
        {
            percentInOrbit -= 1.0f; 
        }
    }

    void Planet::updatePlanet()
    {
        updateOrbitalParams();
    }

    //Drawing Functions
    void Planet::drawPlanetDepiction(Vector2 pos, float radius, float axialTilt)
    {
        DrawCircleLines(pos.x, pos.y, radius, WHITE);

        axialTilt += 90.f; // so that when its 0 degress, its pointing up
        float angleRadians = axialTilt * (PI / 180.0f);

        float x1 = pos.x + cosf(angleRadians) * radius * 2.f;
        float y1 = pos.y + sinf(angleRadians) * radius * 2.f;
        float x2 = pos.x - cosf(angleRadians) * radius * 2.f;
        float y2 = pos.y - sinf(angleRadians) * radius * 2.f;

        DrawLine(pos.x, pos.y, x1, y1, WHITE); // Line from center to edge
        DrawLine(pos.x, pos.y, x2, y2, WHITE); // Line from center to the opposite edge
    }

    void drawStar(Vector2 pos, float Radius)
    {
        float innerRadius = Radius * 0.35f;

        const int points = 10;
        Vector2 vertices[points];

        for (int i = 0; i < points; i++)
        {
            float angle = (i * PI / 5) + PI / 2;
            float r = (i % 2 == 0) ? Radius : innerRadius;
            vertices[i] = {pos.x + r * cosf(angle), pos.y - r * sinf(angle)};
        }

        // Outer Triangles
        DrawTriangle(vertices[0], vertices[1], vertices[9], YELLOW);
        DrawTriangle(vertices[9], vertices[7], vertices[8], YELLOW);
        DrawTriangle(vertices[7], vertices[5], vertices[6], YELLOW);
        DrawTriangle(vertices[5], vertices[3], vertices[4], YELLOW);
        DrawTriangle(vertices[3], vertices[1], vertices[2], YELLOW);
        // Inner Pentagon
        DrawTriangle(vertices[1], vertices[3], vertices[5], YELLOW);
        DrawTriangle(vertices[1], vertices[7], vertices[9], YELLOW);
        DrawTriangle(vertices[1], vertices[7], vertices[9], YELLOW);
        DrawTriangle(vertices[1], vertices[5], vertices[7], YELLOW);
    }

    void Planet::drawPlanetOrbit(Vector2 pos, float radius)
    {
        float semiMajorAxis = distanceFromStar * 50.0f;
        float semiMinorAxis = semiMajorAxis * sqrt(1 - orbitalEccentricity * orbitalEccentricity);

        float focalDistance = semiMajorAxis * orbitalEccentricity;
        Vector2 ellipseCenter = {pos.x - focalDistance, pos.y};

        float angle = 2 * PI * percentInOrbit;

        float r = semiMajorAxis * (1 - orbitalEccentricity * orbitalEccentricity) / (1 + orbitalEccentricity * cos(angle));

        float planetX = r * cos(angle);
        float planetY = r * sin(angle);

        Vector2 planetPos = {ellipseCenter.x + planetX + focalDistance, ellipseCenter.y + planetY};

        drawStar({ellipseCenter.x - focalDistance, ellipseCenter.y}, radius);

        DrawEllipseLines(ellipseCenter.x, ellipseCenter.y, semiMajorAxis, semiMinorAxis, WHITE);
        DrawCircleV(planetPos, radius, WHITE);
    }

    void Planet::drawPlanetOrbitDepiction(Vector2 pos, float radius, float orbitalEccentricity, float distanceFromStar)
    {
        float semiMajorAxis = distanceFromStar * 50.0f;
        float semiMinorAxis = semiMajorAxis * sqrt(1 - orbitalEccentricity * orbitalEccentricity);

        float focalDistance = semiMajorAxis * orbitalEccentricity;
        Vector2 ellipseCenter = {pos.x - focalDistance, pos.y};

        float angle = 2 * PI * percentInOrbit;

        float r = semiMajorAxis * (1 - orbitalEccentricity * orbitalEccentricity) / (1 + orbitalEccentricity * cos(angle));

        float planetX = r * cos(angle);
        float planetY = r * sin(angle);

        Vector2 planetPos = {ellipseCenter.x + planetX + focalDistance, ellipseCenter.y + planetY};

        drawStar({ellipseCenter.x - focalDistance, ellipseCenter.y}, radius);

        DrawEllipseLines(ellipseCenter.x, ellipseCenter.y, semiMajorAxis, semiMinorAxis, WHITE);
        DrawCircleV(planetPos, radius, WHITE);
    }

} // namespace World
