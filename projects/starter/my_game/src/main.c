#include "raylib.h"

int main(void) {
    // Initialize the window
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Move the Block!");

    // Define the camera
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };     // Camera looks at this point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };         // Camera up vector
    camera.fovy = 45.0f;                               // Camera field-of-view
    camera.projection = CAMERA_PERSPECTIVE;            // Camera projection type

    // Define the block's initial position and size
    Vector3 blockPosition = { 0.0f, 0.5f, 0.0f }; // Centered on the XZ plane, slightly above the ground
    const float blockSize = 1.0f;                 // Size of the block

    // Define the movement speed
    const float moveSpeed = 0.1f;

    // Define the boundaries of the space
    const float spaceMinX = -10.0f;
    const float spaceMaxX = 10.0f;
    const float spaceMinZ = -10.0f;
    const float spaceMaxZ = 10.0f;

    // Set the target frames-per-second
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        // Handle input to move the block
        if (IsKeyDown(KEY_D) && blockPosition.x < spaceMaxX) blockPosition.x += moveSpeed;
        if (IsKeyDown(KEY_A) && blockPosition.x > spaceMinX) blockPosition.x -= moveSpeed;
        if (IsKeyDown(KEY_S) && blockPosition.z < spaceMaxZ) blockPosition.z += moveSpeed;
        if (IsKeyDown(KEY_W) && blockPosition.z > spaceMinZ) blockPosition.z -= moveSpeed;

        // Update the camera (optional: make the camera follow the block)
        camera.target = blockPosition;

        // Draw everything
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // Begin 3D mode
            BeginMode3D(camera);
                // Draw the block
                DrawCube(blockPosition, blockSize, blockSize, blockSize, BLUE);
                DrawCubeWires(blockPosition, blockSize, blockSize, blockSize, DARKBLUE);

                // Draw a grid to represent the ground
                DrawGrid(10, 1.0f);
            EndMode3D();

            // Draw 2D elements (e.g., instructions)
            DrawText("Use WASD to move!", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    // Clean up and exit
    CloseWindow();
    return 0;

}
