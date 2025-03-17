import * as raylib from 'raylib'; 
 

export function loadModel(filePath){
  return raylib.LoadModel(filePath); 

}

export function loadTexture(filePath) {
  return raylib.LoadTexture(filePath); 
}



