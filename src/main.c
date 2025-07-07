
#include "raylib.h"
#include "rcamera.h"

#define MAX_COLUMNS 2000
#define MAX_PARTICLES 1000

// Particle structure
typedef struct FallingCube {
    Vector3 position;
    Vector3 velocity;
    Color color;
    float size;
    bool active;
} FallingCube;

static FallingCube fallingCubes[MAX_PARTICLES];

// Initialize particles
static void InitFallingCubes(void) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        fallingCubes[i].active = false;
    }
}

// Update particles
static void UpdateFallingCubes(void) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (fallingCubes[i].active) {
            fallingCubes[i].position.x += fallingCubes[i].velocity.x;
            fallingCubes[i].position.y += fallingCubes[i].velocity.y;
            fallingCubes[i].position.z += fallingCubes[i].velocity.z;

            // Recycle cubes that fall below a certain point
            if (fallingCubes[i].position.y < -10.0f) { // A bit below the ground
                fallingCubes[i].active = false;
            }
        }
    }

    // Emit new cubes
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!fallingCubes[i].active) {
            fallingCubes[i].position = (Vector3){ (float)GetRandomValue(-50, 50), (float)GetRandomValue(50, 100), (float)GetRandomValue(-50, 50) };
            fallingCubes[i].velocity = (Vector3){ 0.0f, -0.5f, 0.0f }; // Falling downwards
            fallingCubes[i].color = (Color){ (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255 }; // Random color
            fallingCubes[i].size = (float)GetRandomValue(1, 5) * 0.1f; // Small random size
            fallingCubes[i].active = true;
            break; // Only emit one cube per frame to control density
        }
    }
}

// Draw particles
static void DrawFallingCubes(void) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (fallingCubes[i].active) {
            DrawCube(fallingCubes[i].position, fallingCubes[i].size, fallingCubes[i].size, fallingCubes[i].size, fallingCubes[i].color);
            DrawCubeWires(fallingCubes[i].position, fallingCubes[i].size, fallingCubes[i].size, fallingCubes[i].size, DARKGRAY);
        }
    }
}

//------------------------------------------------------------------------------------ HOW TO COMPILE and runWHILE DEBUGING IN THE DIRECTORY > gcc main.c -o main -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 
// Program main entry point                                                                
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "embark-openjourney");

    // Load texture for the floor
    Image textureImage = LoadImage("texture.png"); // Load image data into CPU memory (RAM)
    // Define the rectangle for cropping (x, y, width, height)
    Rectangle cropRect = { 0, 0, 256, 256 };
    ImageCrop(&textureImage, cropRect); // Crop the image
    Texture2D floorTexture = LoadTextureFromImage(textureImage); // Load texture from image data
    UnloadImage(textureImage); // Unload image data from CPU memory (RAM)

    // Create a plane mesh and model for the floor
    Mesh floorMesh = GenMeshPlane(1000.0f, 1000.0f, 10, 10);
    Model floorModel = LoadModelFromMesh(floorMesh);
    floorModel.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = floorTexture;

    // Define the camera to look into our 3d world (position, target, up vector)
    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 2.0f, 4.0f };    // Camera position
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    int cameraMode = CAMERA_FIRST_PERSON;

    // Generates some random columns
    float heights[MAX_COLUMNS] = { 0 };
    Vector3 positions[MAX_COLUMNS] = { 0 };
    Color colors[MAX_COLUMNS] = { 0 };

    for (int i = 0; i < MAX_COLUMNS; i++)
    {
        heights[i] = (float)GetRandomValue(40, 150);
        
        // Arrange buildings in a grid with some randomness
        int gridX = i % 40;
        int gridZ = i / 40;
        float posX = (gridX - 20) * 20.0f + (float)GetRandomValue(-2, 2);
        float posZ = (gridZ - 20) * 20.0f + (float)GetRandomValue(-2, 2);
        positions[i] = (Vector3){ posX, heights[i]/2.0f, posZ };

        if (GetRandomValue(0, 10) > 2) {
            unsigned char grayValue = GetRandomValue(40, 150);
            colors[i] = (Color){ grayValue, grayValue, grayValue, 255 };
        } else {
            colors[i] = (Color){ (unsigned char)GetRandomValue(150, 255), 50, 50, 255 };
        }
    }




    DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    InitFallingCubes();                    // Initialize falling cubes

    // Spawn 3 initial cubes near the player's starting position
    for (int i = 0; i < 3; i++) {
        fallingCubes[i].position = (Vector3){ camera.target.x + GetRandomValue(-5, 5), camera.target.y + 20 + GetRandomValue(0, 10), camera.target.z + GetRandomValue(-5, 5) };
        fallingCubes[i].velocity = (Vector3){ 0.0f, -0.5f, 0.0f };
        fallingCubes[i].color = (Color){ (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255 };
        fallingCubes[i].size = (float)GetRandomValue(1, 5) * 0.1f;
        fallingCubes[i].active = true;
    }
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateFallingCubes();                  // Update falling cubes
        // Switch camera mode
        if (IsKeyPressed(KEY_ONE))
        {
            cameraMode = CAMERA_FREE;
            camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        if (IsKeyPressed(KEY_TWO))
        {
            cameraMode = CAMERA_FIRST_PERSON;
            camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        if (IsKeyPressed(KEY_THREE))
        {
            cameraMode = CAMERA_THIRD_PERSON;
            camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        if (IsKeyPressed(KEY_FOUR))
        {
            cameraMode = CAMERA_ORBITAL;
            camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        // Switch camera projection
        if (IsKeyPressed(KEY_P))
        {
            if (camera.projection == CAMERA_PERSPECTIVE)
            {
                // Create isometric view
                cameraMode = CAMERA_THIRD_PERSON;
                // Note: The target distance is related to the render distance in the orthographic projection
                camera.position = (Vector3){ 0.0f, 2.0f, -100.0f };
                camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
                camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
                camera.projection = CAMERA_ORTHOGRAPHIC;
                camera.fovy = 20.0f; // near plane width in CAMERA_ORTHOGRAPHIC
                CameraYaw(&camera, -135 * DEG2RAD, true);
                CameraPitch(&camera, -45 * DEG2RAD, true, true, false);
            }
            else if (camera.projection == CAMERA_ORTHOGRAPHIC)
            {
                // Reset to default view
                cameraMode = CAMERA_THIRD_PERSON;
                camera.position = (Vector3){ 0.0f, 2.0f, 10.0f };
                camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
                camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
                camera.projection = CAMERA_PERSPECTIVE;
                camera.fovy = 60.0f;
            }
        }

        // Update camera computes movement internally depending on the camera mode
        // Some default standard keyboard/mouse inputs are hardcoded to simplify use
        // For advanced camera controls, it's recommended to compute camera movement manually
        UpdateCamera(&camera, cameraMode);                  // Update camera

/*
        // Camera PRO usage example (EXPERIMENTAL)
        // This new camera function allows custom movement/rotation values to be directly provided
        // as input parameters, with this approach, rcamera module is internally independent of raylib inputs
        UpdateCameraPro(&camera,
            (Vector3){
                (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))*0.1f -      // Move forward-backward
                (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))*0.1f,    
                (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))*0.1f -   // Move right-left
                (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))*0.1f,
                0.0f                                                // Move up-down
            },
            (Vector3){
                GetMouseDelta().x*0.05f,                            // Rotation: yaw
                GetMouseDelta().y*0.05f,                            // Rotation: pitch
                0.0f                                                // Rotation: roll
            },
            GetMouseWheelMove()*2.0f);                              // Move to target (zoom)
*/
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLUE);

            BeginMode3D(camera);

                DrawModel(floorModel, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, WHITE); // Draw textured ground
                DrawGrid(1000, 1.0f); // Draw a grid on the ground

                // Draw some cubes around
                for (int i = 0; i < MAX_COLUMNS; i++)
                {
                    DrawCube(positions[i], 2.5f, heights[i], 2.5f, colors[i]);
                    DrawCubeWires(positions[i], 2.5f, heights[i], 2.5f, DARKGRAY);
                }

                // Draw player cube   I think this is where I have to enter my character
		//
		//
		//
		//
		//
		//
		//
                if (cameraMode == CAMERA_THIRD_PERSON)
                {
                    DrawCube(camera.target, 0.5f, 0.5f, 0.5f, PURPLE);
                    DrawCubeWires(camera.target, 0.5f, 0.5f, 0.5f, DARKPURPLE);
                }

                DrawFallingCubes(); // Draw falling cubes

            EndMode3D();

            // Draw info boxes
            DrawRectangle(5, 5, 330, 100, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines(5, 5, 330, 100, BLUE);

            DrawText("Camera controls:", 15, 15, 10, BLACK);
            DrawText("- Move keys: W, A, S, D, Space, Left-Ctrl", 15, 30, 10, BLACK);
            DrawText("- Look around: arrow keys or mouse", 15, 45, 10, BLACK);
            DrawText("- Camera mode keys: 1, 2, 3, 4", 15, 60, 10, BLACK);
            DrawText("- Zoom keys: num-plus, num-minus or mouse scroll", 15, 75, 10, BLACK);
            DrawText("- Camera projection key: P", 15, 90, 10, BLACK);

            DrawRectangle(600, 5, 195, 100, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines(600, 5, 195, 100, BLUE);

            DrawText("Camera status:", 610, 15, 10, BLACK);
            DrawText(TextFormat("- Mode: %s", (cameraMode == CAMERA_FREE) ? "FREE" :
                                              (cameraMode == CAMERA_FIRST_PERSON) ? "FIRST_PERSON" :
                                              (cameraMode == CAMERA_THIRD_PERSON) ? "THIRD_PERSON" :
                                              (cameraMode == CAMERA_ORBITAL) ? "ORBITAL" : "CUSTOM"), 610, 30, 10, BLACK);
            DrawText(TextFormat("- Projection: %s", (camera.projection == CAMERA_PERSPECTIVE) ? "PERSPECTIVE" :
                                                    (camera.projection == CAMERA_ORTHOGRAPHIC) ? "ORTHOGRAPHIC" : "CUSTOM"), 610, 45, 10, BLACK);
            DrawText(TextFormat("- Position: (%06.3f, %06.3f, %06.3f)", camera.position.x, camera.position.y, camera.position.z), 610, 60, 10, BLACK);
            DrawText(TextFormat("- Target: (%06.3f, %06.3f, %06.3f)", camera.target.x, camera.target.y, camera.target.z), 610, 75, 10, BLACK);
            DrawText(TextFormat("- Up: (%06.3f, %06.3f, %06.3f)", camera.up.x, camera.up.y, camera.up.z), 610, 90, 10, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(floorTexture); // Unload texture
    UnloadModel(floorModel);     // Unload model
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
