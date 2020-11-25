#pragma once

#include <stdlib.h>     /* srand, rand */
#include <time.h>

#include "gameObject.h"
#include "sprite.h"
#include "tile.h"
#include "powerup.h"

#include "MouseD.h"

#define PI 3.1415926

typedef struct Car {
  struct gameObject gO;
  //struct gameObject prev;
  struct SpriteBmp *sp;
  struct SpriteBmp *light;
  bool l;

  int acelerar;
  int travar;
  int virar_esquerda;
  int virar_direita;

  bool applyPowerUp;

  bool ai;
  double sensores[5];

  float max_laps;
  float laps;

  PowerUp * pu;
  Vector dir;

  uint16_t hold_pu;

  int ticks;

  Item *items;
  size_t n_items;

  uint32_t turtle_hit;
  uint32_t  banana_hit;

  uint8_t tecla;



}Car;

Car carInit (double x, double y, double vx, double vy, double ang, struct SpriteBmp *spr, struct SpriteBmp *l, bool ai);

void desenharCarro(struct Car *carro);

void desenharCarroCamera(struct Car *carro , struct Camera * cm);

void receberTecla(struct Car *carro , uint8_t tecla);

void receberMouse(struct Car * carro, struct Mouse *m);

void applyPowerUp(struct Car * carro);

void updatePowerUp(struct Car * carro);

void updateCarro(struct Car *carro);

void hitItem(struct Car * carro , uint8_t id);

void insertItem(struct Car * carro , size_t id, Vector dir);

void removeItem(struct Car * carro ,  size_t indice);

void freeCarro(struct Car* carro);

void readSensores(struct Car* carro, struct QuadTreeObs* qt);

void carroAi(struct Car * carro);

void carroLights(struct Car * carro, struct Camera *cm);
