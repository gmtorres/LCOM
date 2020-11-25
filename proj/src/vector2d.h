#pragma once

#include <math.h>

typedef struct Vector {
  double x;
  double y;
}Vector;

Vector v (double x, double y) ;
void addVectors (struct Vector* v1, struct Vector* v2);
void subVectors (struct Vector* v1, struct Vector* v2);
void multVectors (struct Vector* v1, double a);
double normaVector (struct Vector* v1);
Vector normalVector(struct Vector* v1);
double prodEscalar (struct Vector* v1, struct Vector* v2);
Vector projVector (struct Vector* v1, struct Vector* v2);
Vector multVectorMatriz (struct Vector* v1, double a, double b, double c, double d);
Vector rotateVector (struct Vector *v1, double ang);
