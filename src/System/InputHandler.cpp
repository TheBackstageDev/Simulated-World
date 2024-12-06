#include "../src/Headers/System/InputHandler.hpp"

#define CAM_SPEED 100

namespace System_Input
{
    void System_Input::handleCameraInput(Camera2D& camera)
    {
        float deltaTime = GetFrameTime();
        if (IsKeyDown(KEY_W))
            camera.target.y -= CAM_SPEED / camera.zoom * deltaTime;
        if (IsKeyDown(KEY_S))
            camera.target.y += CAM_SPEED / camera.zoom * deltaTime;
        if (IsKeyDown(KEY_A))
            camera.target.x -= CAM_SPEED / camera.zoom * deltaTime;
        if (IsKeyDown(KEY_D))
            camera.target.x += CAM_SPEED / camera.zoom * deltaTime;

        camera.zoom += GetMouseWheelMove() * 0.1f;
        if (camera.zoom < 0.1f)
            camera.zoom = 0.1f;
    }
} // namespace System_Input
