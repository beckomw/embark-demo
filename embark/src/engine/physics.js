import * as raylib from 'raylib'; 

export function menuScene() {
  raylib.DrawText("Welcome to the Game!", 200, 200, 20, raylib.WHITE); 
  raylib.DrawText("Press ENTER to Start", 200, 250, 20, raylib.GRAY); 

  if (raylib.IsKeyPressed(raylib.KEY_ENTER)) {
    return 'world'; 

  }

  return 'menu'; 

}
