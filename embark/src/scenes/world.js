

import * as raylib from 'raylib';
import { renderWorld } from '../engine/renderer.js';

const camera = raylib.Camera3D({
    position: { x: 0, y: 10, z: 10 },
    target: { x: 0, y: 0, z: 0 },
    up: { x: 0, y: 1, z: 0 },
    fovy: 45,
    type: raylib.CAMERA_PERSPECTIVE
});

const objects = [
    { model: null, position: { x: 0, y: 0, z: 0 }, color: raylib.RED }
];

export function worldScene() {
    objects.forEach(obj => {
        if (!obj.model) obj.model = raylib.LoadModel('./assets/models/cube.obj');
    });

    renderWorld(camera, objects);

    if (raylib.IsKeyPressed(raylib.KEY_ESCAPE)) {
        return 'menu';
    }

    return 'world';
}

