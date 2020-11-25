#pragma once



#include "quadtree.h"
#include "sprite.h"
#include "vector2d.h"
#include "v_gr.h"

#include "camera.h"
#include "tile.h"
#include "car.h"
#include "numbers.h"

#include "MouseD.h"

static SpriteBmp * carros[2];
static SpriteBmp * lights[2];

static bool init = false;

typedef struct Pista{

  uint16_t x_tiles;
  uint16_t y_tiles;
  Tile * mapa;

  gameObject meta1;
  gameObject meta2;

  //FILE * fp;

  uint16_t n_carros;
  Car *carros;

  uint16_t laps;

  QuadTreeObs obstacles;
  QuadTreePowerUpSpawn powerUpSpawns;

  Camera cm;

  uint32_t ticks;
  bool time;
  uint8_t date[6];

  bool multiplayer;

  bool end;

}Pista;

Pista *pistaInit(char * filename , bool mp, bool server);
void freePista(struct Pista *p);

void desenharNivel(struct Pista * p);
void desenharNivelCamera(struct Pista * p , struct Camera * cm);

void getInputKbd(struct Pista * p, uint8_t tecla); // todo o input so afeta o car 0, os outros carros sao inimigos, pelo que no mapa é definido o numero de inimigos
void getInputSp1(struct Pista *p , uint8_t ch); //caso a pista esteja em multiplayer todo o input afeta o carro 1
void getInputMouse(struct Pista * p, struct Mouse *m);
void getDate(struct Pista * p, uint8_t date[6]);


void desenharPista(struct Pista *p);
void updatePista(struct Pista *p);

void checkmeta(struct Pista *p);
void checkCollisionCar(struct Pista *p , struct Car *car);
void checkCollisionCars(struct Pista *p);

void orderPosition2(struct Pista * p, int * v);

void catchPowerUps(struct Pista *p);
bool checkEndOfRace(struct Pista * p);

int getPosition(struct Pista * p);
