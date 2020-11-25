#include "obstacle.h"

void initiateObstaculos(){
  obstaculos[0] = sBmpInit("/home/lcom/labs/proj/resources/images/objects/barrier_red.bmp"); //a
  obstaculos[1] = sBmpInit("/home/lcom/labs/proj/resources/images/objects/barrier_red_vertical.bmp"); //b
  obstaculos[2] = sBmpInit("/home/lcom/labs/proj/resources/images/objects/barreira_30.bmp");  //c
  obstaculos[3] = sBmpInit("/home/lcom/labs/proj/resources/images/objects/barreira_60.bmp"); //d
  obstaculos[4] = sBmpInit("/home/lcom/labs/proj/resources/images/objects/barreira_120.bmp"); //e
  obstaculos[5] = sBmpInit("/home/lcom/labs/proj/resources/images/objects/barreira_150.bmp"); //f
  obstaculos[6] = sBmpInit("/home/lcom/labs/proj/resources/images/objects/barrel_blue.bmp"); //g
  obstaculos[7] = sBmpInit("/home/lcom/labs/proj/resources/images/objects/tribune_empty.bmp"); //h
  obstaculos[8] = sBmpInit("/home/lcom/labs/proj/resources/images/objects/rock1.bmp"); //i
  obstaculos[9] = sBmpInit("/home/lcom/labs/proj/resources/images/objects/cone_straight.bmp"); //j
  obstaculos[10] = sBmpInit("/home/lcom/labs/proj/resources/images/objects/tree_large.bmp"); //k

}

void freeObstaculos(){
  for (unsigned int i = 0; i < sizeof(obstaculos)/sizeof(obstaculos[0]); i++) {
    freeBmp(obstaculos[i]);
  }
}

#define PI 3.14156
Obstacle obstacleInit(int id, double x, double y){
    Obstacle obj;
    obj.spr = obstaculos[id];
    uint8_t w;
    uint16_t h;
    if(id == 0 || id == 1){
      w = obj.spr->bmp->bitmapInfoHeader.width;
      h = obj.spr->bmp->bitmapInfoHeader.height;
      obj.gameObs = gameObjectRectangle(x ,y ,0,0,w,h,0,true,10000);
    }
    if(id == 2){
      w = obstaculos[0]->bmp->bitmapInfoHeader.width;
      h = obstaculos[0]->bmp->bitmapInfoHeader.height;
      obj.gameObs = gameObjectRectangle(x ,y ,0,0,w,h,atan(0.5),true,10000);
    }
    if(id == 3){
      w = obstaculos[0]->bmp->bitmapInfoHeader.width;
      h = obstaculos[0]->bmp->bitmapInfoHeader.height;
      obj.gameObs = gameObjectRectangle(x ,y ,0,0,w,h,atan(2),true,10000);
    }
    if(id == 4){
      w = obstaculos[0]->bmp->bitmapInfoHeader.width;
      h = obstaculos[0]->bmp->bitmapInfoHeader.height;
      obj.gameObs = gameObjectRectangle(x ,y ,0,0,w,h,-atan(2),true,10000);
    }
    if(id == 5){
      w = obstaculos[0]->bmp->bitmapInfoHeader.width;
      h = obstaculos[0]->bmp->bitmapInfoHeader.height;
      obj.gameObs = gameObjectRectangle(x ,y ,0,0,w,h,-atan(0.5),true,10000);
    }
    if(id == 6){
      w = obj.spr->bmp->bitmapInfoHeader.width;
      h = obj.spr->bmp->bitmapInfoHeader.height;
      obj.gameObs = gameObjectCircle(x,y,0,0,w/2,1,true,10000);
    }
    if( id == 7 || id == 8 || id == 9 || id == 10 ){
      w = obj.spr->bmp->bitmapInfoHeader.width;
      h = obj.spr->bmp->bitmapInfoHeader.height;
      obj.gameObs = gameObjectRectangle(x ,y ,0,0,w,h,30,true,10000);
    }
    return obj;
}

void drawObstacle(struct Obstacle * obs,struct Camera * cm){
  Vector desl = cameraDeslocamento(cm);
  if(obs->gameObs.ang == 0)
    drawBitmapFast(obs->spr->bmp, obs->gameObs.pos.x - desl.x - *obs->spr->width/2, obs->gameObs.pos.y - desl.y - *obs->spr->height/2 );
  else
    drawBitmap(obs->spr->bmp, obs->gameObs.pos.x - desl.x - *obs->spr->width/2, obs->gameObs.pos.y - desl.y - *obs->spr->height/2 );
}
