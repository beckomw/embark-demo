#include "raylib.h"

// Function to scale UV coordinates
void ScaleMeshUVs(Mesh *mesh, float scaleX, float scaleY) {
    for (int i = 0; i < mesh->vertexCount; i++) {
        mesh->texcoords[i * 2] *= scaleX;     // Scale U coordinate
        mesh->texcoords[i * 2 + 1] *= scaleY; // Scale V coordinate
    }
}

int main(void) {
    // Initialize the window
    const int screenWidth = 1200;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Scrappy's Revenge");

    // Define the camera
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 15.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera target
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector
    camera.fovy = 45.0f;                                // Camera field-of-view
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    // Load the floor texture
    Texture2D floorTexture = LoadTexture("resources/floortile.png");
    if (floorTexture.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load floor texture!");
    }

    // Define the terrain boundaries
    const float spaceMinX = -10.0f;
    const float spaceMaxX = 10.0f;
    const float spaceMinZ = -10.0f;
    const float spaceMaxZ = 10.0f;

    // Calculate floor size based on boundaries
    float floorWidth = spaceMaxX - spaceMinX;  // 20.0f
    float floorHeight = spaceMaxZ - spaceMinZ; // 20.0f

    // Create a floor mesh and model
    Mesh floorMesh = GenMeshPlane(floorWidth, floorHeight, 20, 20); // Width, height, and subdivisions

    // Scale UVs to match texture to floor size
    float textureTileSize = 1.0f; // Size of each texture tile in game units
    float uvScaleX = floorWidth / textureTileSize;  // 20.0f / 1.0f = 20.0f
    float uvScaleY = floorHeight / textureTileSize; // 20.0f / 1.0f = 20.0f
    ScaleMeshUVs(&floorMesh, uvScaleX, uvScaleY);

    Model floorModel = LoadModelFromMesh(floorMesh); // Create the model
    if (floorModel.meshCount == 0) {
        TraceLog(LOG_ERROR, "Failed to create floor model!");
    }
    floorModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = floorTexture;

    // Define the floor position
    Vector3 floorPosition = { 0.0f, 0.0f, 0.0f };

    // Define the player block
    Vector3 blockPosition = { 0.0f, 0.5f, 0.0f };       // Player position (Y is height)
    const float blockSize = 1.0f;                       // Size of the player block
    const float moveSpeed = 0.1f;                       // Speed of player movement

    // Set the target frames-per-second
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        // Handle input to move the block
        if (IsKeyDown(KEY_D) && blockPosition.x < spaceMaxX) blockPosition.x += moveSpeed;
        if (IsKeyDown(KEY_A) && blockPosition.x > spaceMinX) blockPosition.x -= moveSpeed;
        if (IsKeyDown(KEY_S) && blockPosition.z < spaceMaxZ) blockPosition.z += moveSpeed;
        if (IsKeyDown(KEY_W) && blockPosition.z > spaceMinZ) blockPosition.z -= moveSpeed;

        // Update the camera to follow the player
        camera.target = blockPosition;

        // Draw everything
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // Begin 3D mode
            BeginMode3D(camera);
                // Draw the floor
                DrawModel(floorModel, floorPosition, 1.0f, WHITE);

                // Draw the player block
                DrawCube(blockPosition, blockSize, blockSize, blockSize, BLUE);
                DrawCubeWires(blockPosition, blockSize, blockSize, blockSize, DARKBLUE);
            EndMode3D();

            // Draw 2D elements (e.g., instructions)
            DrawText("Use WASD to move!", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    // Unload the floor model and texture
    UnloadModel(floorModel);
    UnloadTexture(floorTexture);

    // Close the window
    CloseWindow();
    return 0;
}
