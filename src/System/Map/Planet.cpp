#include "../src/Headers/System/Map/Planet.hpp"
#include <raymath.h>

namespace World
{
    Planet::Planet(float axialTilt, float orbitalEccentricity, float distanceFromStar, float orbitalPeriod, float rotationPeriod)
    : axialTilt(axialTilt), orbitalEccentricity(orbitalEccentricity), distanceFromStar(distanceFromStar), orbitalPeriod(orbitalPeriod), rotationPeriod(rotationPeriod)
    {   
    }

    Planet::Planet()
    : axialTilt(23.4f), orbitalEccentricity(0.01f), distanceFromStar(1.0f), orbitalPeriod(365.f), rotationPeriod(24.f)
    {}

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

    void Planet::drawPlanetOrbitDepiction(Vector2 pos, float radius, float orbitalEccentricity, float distanceFromStar)
    {
        float semiMajorAxis = distanceFromStar * 50.f;
        float semiMinorAxis = semiMajorAxis * sqrt(1 - orbitalEccentricity * orbitalEccentricity);

        float focalDistance = semiMajorAxis * orbitalEccentricity;
        Vector2 ellipseCenter = {pos.x - semiMajorAxis, pos.y};

        drawStar({ellipseCenter.x - focalDistance, ellipseCenter.y}, radius);

        DrawEllipseLines(ellipseCenter.x, ellipseCenter.y, semiMajorAxis, semiMinorAxis, WHITE);
        DrawCircleLines(pos.x, pos.y, radius, WHITE);
    }

} // namespace World
