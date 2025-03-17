
import { menuScene } from './scenes/menu.js';
import { worldScene } from './scenes/world.js';

let currentScene = 'menu';

function runGame() {
    while (!raylib.WindowShouldClose()) {
        raylib.BeginDrawing();
        raylib.ClearBackground(raylib.BLACK);

        if (currentScene === 'menu') {
            currentScene = menuScene();
        } else if (currentScene === 'world') {
            currentScene = worldScene();
        }

        raylib.EndDrawing();
    }
}

runGame();
