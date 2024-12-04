#include <raylib.h>

int main()
{
    constexpr int width = 800;
    constexpr int height = 800;


    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, "Raylib Window");

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        EndDrawing();
    }

    return 0;
}