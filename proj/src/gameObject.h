#pragma once

#include "vector2d.h"
#include "rectangle.h"
#include "circle.h"
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <float.h>
#include <v_gr.h>


typedef struct gameObject{
  int type; // 0 = rectangle , 1 = circle
  bool fixed; // 1 = statcionary , 0 = moving object
  union{
    struct Rectangle r;
    struct Circle c;
  } shape;

  Vector pos;
  Vector speed;

  double atrito;

  bool angulo;
  double ang;

  Vector prev_pos;
  Vector prev_speed;
  double pre_ang;

  //Sprite image;

}gameObject;

gameObject gameObjectRectangle(double x, double y , double vx, double vy, double comp, double larg, double ang,bool fixed , double atrito);
gameObject gameObjectCircle(double x, double y , double vx, double vy, double r, double ang,bool fixed , double atrito);
void updatePosition (struct gameObject* gOb);
void gameObjectGoBack(struct gameObject * gOb);
void acelerar (struct gameObject* gObj, double k);
void rodarEsquerda(struct gameObject* gObj);
void rodarDireita(struct gameObject* gObj);
Vector getAtrito(struct gameObject* gObj);
void travar (struct gameObject* gObj);

bool collision (struct gameObject* gObj1, struct gameObject* gObj2);
Vector ballRotation(struct Vector posRect, struct Vector posCircle, double ang);
bool collisionRectangles(struct gameObject* gObj1, struct gameObject* gObj2);
bool collisionRectangleCircle(struct gameObject* gObj1, struct gameObject* gObj2);
bool collisionCircles(struct gameObject* gObj1, struct gameObject* gObj2);

void checkHandleCollision (struct gameObject* gObj1, struct gameObject* gObj2);
bool checkHandleCollisionRectangles(struct gameObject* gObj1, struct gameObject* gObj2);
bool checkHandlecollisionRectangleCircle(struct gameObject* gObj1, struct gameObject* gObj2);
