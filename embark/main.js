import raylib from "raylib"; // Importing the default object 


const rl = raylib; // Assining the imported object to 'rl' for easier usage 

// Initialize a window 
rl.InitWindow(800, 600, "Embark Raylib Demo"); 
rl.SetTargetFPS(60); 

// Main game loop 

while(!rl.WindowShouldClose()){
  rl.BeginDrawing(); 
  rl.ClearBackground(rl.RAYWHITE); 
  rl.DrawText("Hello, Raylib with JavaScript!", 200, 200, 22, rl.BLACK);
  rl.EndDrawing(); 
}


// Close the window when finished 

rl.CloseWindow(); 





