#include "../src/Headers/System/InputHandler.hpp"
#include "../src/Headers/Utils/InputDefinitions.hpp"
#include "../src/Headers/Utils/WindowUtils.hpp"
#include "../src/Headers/Utils/CameraUtils.hpp"
#include "../src/Headers/Utils/WorldDefinitions.hpp"
#include "../src/Headers/Utils/WorldUtils.hpp"

#define CAM_SPEED 100

namespace System_Input
{
    void System_Input::handleCameraInput(Camera2D& camera)
    {
        float deltaTime = GetFrameTime();
        if (IsKeyDown(MOVE_UP))
            camera.target.y -= CAM_SPEED / camera.zoom * deltaTime;
        if (IsKeyDown(MOVE_DOWN))
            camera.target.y += CAM_SPEED / camera.zoom * deltaTime;
        if (IsKeyDown(MOVE_LEFT))
            camera.target.x -= CAM_SPEED / camera.zoom * deltaTime;
        if (IsKeyDown(MOVE_RIGHT))
            camera.target.x += CAM_SPEED / camera.zoom * deltaTime;

        camera.zoom += GetMouseWheelMove() * 0.1f;
        if (camera.zoom < 0.1f)
            camera.zoom = 0.1f;
    }

    bool System_Input::handleSimulationInput(Camera2D &camera)
    {
        //Related to AI and stuff
        if (IsKeyPressed(INPUT_PLACE_CIV))
        {
            return true; // ToBegin();
        }
    }
} // namespace System_Input
