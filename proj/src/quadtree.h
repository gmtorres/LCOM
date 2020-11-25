#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "vector2d.h"
#include "list.h"
#include "obstacle.h"

#include "powerup.h"

//x e y top left corner
// width e height
typedef struct Boundary{
  Vector posi;
  double  width;
  double  height;
}Boundary;

Boundary boundaryInit(double  x, double  y , double  width, double  height);

bool contains(Boundary b, Vector point);

bool intersect(Boundary b1 , Boundary b2);

typedef struct QuadTreeObs{

  struct Boundary boundary;

  int max_obstacles;
  int n_obstacles;

  Obstacle * obstacles;

  struct QuadTreeObs * qt1;
  struct QuadTreeObs * qt2;
  struct QuadTreeObs * qt3;
  struct QuadTreeObs * qt4;

  bool divided;

} QuadTreeObs;


QuadTreeObs quadtreeObsInit(double  x, double  y , double  width, double  height , int n_obstacles);

void quadtreeObsInsertObs(QuadTreeObs *qt , Obstacle);

void quadtreeObsFree(QuadTreeObs *qt);

void quadtreeObsQuery(QuadTreeObs * qt , double x, double y , double width, double height, List * l);







typedef struct QuadTreePowerUpSpawn{

  struct Boundary boundary;

  int max_PowerUpSpawns;
  int n_PowerUpSpawns;

  PowerUpSpawn * obstacles;

  struct QuadTreePowerUpSpawn * qt1;
  struct QuadTreePowerUpSpawn * qt2;
  struct QuadTreePowerUpSpawn * qt3;
  struct QuadTreePowerUpSpawn * qt4;

  bool divided;

} QuadTreePowerUpSpawn;


QuadTreePowerUpSpawn quadtreePowerUpSpawnInit(double  x, double  y , double  width, double  height , int n_obstacles);

void quadtreePowerUpSpawnInsertPowerUpSpawn(QuadTreePowerUpSpawn *qt , PowerUpSpawn p);

void quadtreePowerUpSpawnFree(QuadTreePowerUpSpawn *qt);

void quadtreePowerUpSpawnQuery(QuadTreePowerUpSpawn * qt , double x, double y , double width, double height, List * l);
