#pragma once

#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <stdbool.h>

#include "sprite.h"
#include "vector2d.h"
#include "gameObject.h"
#include "camera.h"

static SpriteBmp * powups [3];
static SpriteBmp * items[2];





typedef struct PowerUp {
  int id;
  uint16_t max_hold_time;
  uint16_t max_util_time;
  bool activated;
  struct SpriteBmp *spr;
  gameObject gObj;
}PowerUp;

PowerUp *PowerUpInit (int i ,double x, double y);
void changeCoordinates (struct PowerUp *p, uint16_t x, uint16_t y);
void drawPowerUp (struct PowerUp *p);
void drawPowerUpCamera (struct PowerUp *p , struct Camera * cm);
void activatePowerUp(struct PowerUp *p);
void initiatePowerUps();
void freePowerUps();







typedef struct PowerUpSpawn{

  gameObject gObj;
  PowerUp *pu;
  int waiting_time;

}PowerUpSpawn;

PowerUpSpawn powerUpSpawnInit(double x, double y);
void generatePowerUp(struct PowerUpSpawn * pus);

void drawPowerUpSpawnCamera(struct PowerUpSpawn * pus,  struct Camera *cm);







typedef struct Item{

  uint8_t id;

  struct SpriteBmp *spr;
  gameObject gObj;

  uint32_t lifetime;

  gameObject * carro; //car associated with the item, this item cant collide with the car, until it gets out of the car range, in this case the pointer is NULL

}Item;

void initiateItems();
void freeItems();
void drawItem(struct Item *it, struct Camera * cm);
Item itemInit(unsigned int id, double x, double y, Vector dir, struct  gameObject *c);
