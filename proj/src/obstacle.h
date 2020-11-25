#pragma once

#include "gameObject.h"
#include "sprite.h"
#include "camera.h"

#define PI2 3.141569

static SpriteBmp * obstaculos[11];

typedef struct Obstacle{

    struct gameObject gameObs;
    struct SpriteBmp *spr;


}Obstacle;

void initiateObstaculos();
void freeObstaculos();

Obstacle obstacleInit(int id,double x, double y);

void drawObstacle(struct Obstacle * obs, struct Camera * cm);
