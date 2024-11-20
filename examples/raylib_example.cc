
#include "raylib.h"

int main(void)
{
    // Define the target resolution (128x128)
    const int targetWidth = 128;
    const int targetHeight = 128;

    // Initialize the window at a rescalable resolution
    const int initialScreenWidth = 512;
    const int initialScreenHeight = 512;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);  // This makes the window resizable
    InitWindow(initialScreenWidth, initialScreenHeight, "raylib - Rescalable 128x128 render");

    // Create a render texture for the 128x128 scene
    RenderTexture2D target = LoadRenderTexture(targetWidth, targetHeight);

    SetTargetFPS(60);

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Get current window dimensions
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        // Calculate the largest square that fits within the window
        int scaleSize = (screenWidth < screenHeight) ? screenWidth : screenHeight;

        // Center the square on the screen
        int posX = (screenWidth - scaleSize) / 2;
        int posY = (screenHeight - scaleSize) / 2;

        // Begin drawing to the render texture at 128x128 resolution
        BeginTextureMode(target);
        ClearBackground(RAYWHITE);

        // Render your scene at 128x128 resolution
        DrawText("HELLO RICHARD.... I ", 10, 10, 5, DARKGRAY);

        EndTextureMode();

        // Begin drawing to the screen (rescaled)
        BeginDrawing();
        ClearBackground(BLACK);  // Clear to black for contrast

        // Draw the render texture, scaled up and centered
        DrawTexturePro(target.texture,
                      (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height },  // Source (inverted y-axis)
                      (Rectangle){ posX, posY, scaleSize, scaleSize },  // Destination (scaled and centered)
                      (Vector2){ 0, 0 }, 0.0f, WHITE);  // No rotation, white tint

        EndDrawing();
    }

    // Clean up
    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}
