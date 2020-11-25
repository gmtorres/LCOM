#include "powerup.h"


PowerUp * PowerUpInit (int i ,double x, double y){
  struct PowerUp *p = (PowerUp *)malloc(sizeof(PowerUp));
  p->id = i;
  switch(p->id){
    case 0:
      p->max_hold_time = 600;
      p->max_util_time = 180;
      break;
    case 1:
      p->max_hold_time = 600;
      p->max_util_time = 1;
      break;
    case 2:
      p->max_hold_time = 600;
      p->max_util_time = 1;
      break;
    default:break;
  }
  p->gObj = gameObjectCircle(x,y,0,0,32,0,false,0);
  p->activated = false;
  p->spr = powups[i];
  return p;
}


void drawPowerUp(struct PowerUp *p) {
  drawSpriteBmp(p->spr, p->gObj.pos.x, p->gObj.pos.y);
}

void drawPowerUpCamera (struct PowerUp *p , struct Camera * cm){
  struct Vector desl = cameraDeslocamento(cm);
  drawBitmap(p->spr->bmp, p->gObj.pos.x - desl.x, p->gObj.pos.y - desl.y);
}


void changeCoordinates (struct PowerUp *p, uint16_t x, uint16_t y) {
  p->gObj.pos.x = x;
  p->gObj.pos.y = y;
}

void activatePowerUp(struct PowerUp *p) {
  p->activated = true;
}

void initiatePowerUps() {
  memset(powups , 0 , sizeof(powups));
  powups[0] = sBmpInit("/home/lcom/labs/proj/resources/images/utilities/power_up_speed.bmp");
  powups[1] = sBmpInit("/home/lcom/labs/proj/resources/images/utilities/power_up_turtle.bmp");
  powups[2] = sBmpInit("/home/lcom/labs/proj/resources/images/utilities/power_up_banana.bmp");
}

void freePowerUps() {
  for (unsigned int i = 0; i < sizeof(powups)/sizeof(powups[0]); i++) {
    if(powups[i] != NULL){
      freeBmp(powups[i]);
      powups[i] = NULL;
    }
  }
}

PowerUpSpawn powerUpSpawnInit(double x, double y){
  PowerUpSpawn pus;
  pus.gObj = gameObjectCircle(x,y,0,0,32,0,true , 0);
  pus.pu = NULL;
  pus.waiting_time = -1;
  return pus;
}
void generatePowerUp(struct PowerUpSpawn * pus){
  static int v = 0;
  if(!v){
    srand(time(NULL));
    v++;
  }
  unsigned int r = (int)rand() % 3;
  pus->pu = PowerUpInit(r,pus->gObj.pos.x , pus->gObj.pos.y);

}

void drawPowerUpSpawnCamera(struct PowerUpSpawn * pus,  struct Camera *cm){
  if(pus->pu != NULL)
    drawPowerUpCamera(pus->pu , cm);
}




Item itemInit(unsigned int id, double x, double y, Vector dir , struct gameObject *c) {
  Item it;
  it.id = id;
  it.lifetime = 600;
  it.spr = items[id];
  switch (id) {
    case 0:
    multVectors (&dir, (normaVector(&c->speed)*0 +15)/normaVector(&dir));
    break;
    case 1:
    multVectors (&dir, 0.0001/normaVector(&dir));
    break;
    default:
    multVectors (&dir, 3/normaVector(&dir));
    break;
  }
  it.gObj = gameObjectCircle(x,y, dir.x, dir.y, (it.spr->bmp->bitmapInfoHeader.width + it.spr->bmp->bitmapInfoHeader.height)/4, atan(dir.y/dir.x), false , 0);
  it.carro = c;
  return it;
}

void drawItem(struct Item *it, struct Camera * cm){
  struct Vector desl = cameraDeslocamento(cm);
  drawBitmap(it->spr->bmp , it->gObj.pos.x - it->gObj.shape.c.r - desl.x , it->gObj.pos.y - it->gObj.shape.c.r - desl.y);
}

void initiateItems(){
  memset(items , 0 , sizeof(items));
  items[0] = sBmpInit("/home/lcom/labs/proj/resources/images/utilities/turtle.bmp");
  items[1] = sBmpInit("/home/lcom/labs/proj/resources/images/utilities/banana.bmp");

}
void freeItems(){
  for (unsigned int i = 0; i < sizeof(items)/sizeof(items[0]); i++) {
    if(items[i] != NULL){
      freeBmp(items[i]);
      items[i] = NULL;
    }
  }
}
