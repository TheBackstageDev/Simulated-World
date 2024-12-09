#include "../src/Headers/Utils/WindowUtils.hpp"

namespace System_Utils
{
    void System_Utils::updateScales()
    {
        scaleX = (float)GetScreenWidth() / windowWidth;
        scaleY = (float)GetScreenHeight() / windowHeight;
        windowWidth = (float)GetScreenWidth();
        windowHeight = (float)GetScreenHeight();
    }

    float System_Utils::getCurrentWindowScaleOffset(Vector2 referenceSize)
    {
        int screenHeight = GetScreenHeight();
        int screenWidth = GetScreenWidth();

        float scaleX = static_cast<float>(screenWidth) / referenceSize.x;
        float scaleY = static_cast<float>(screenHeight) / referenceSize.y;
        return std::min(scaleX, scaleY);
    }

    float System_Utils::getScaleOffset(Vector2 referenceSizeA, Vector2 referenceSizeB)
    {
        float scaleX = static_cast<float>(referenceSizeA.x) / referenceSizeB.x;
        float scaleY = static_cast<float>(referenceSizeA.y) / referenceSizeB.y;
        return std::min(scaleX, scaleY);
    }

    Vector2 System_Utils::getScreenCenter()
    {
        return {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
    }

    void System_Utils::scaleRectangle(Rectangle &rect)
    {
        rect.width *= scaleX;
        rect.height *= scaleY;
        rect.x *= scaleX;
        rect.y *= scaleY;
    }
}   // namespace System_Utils
