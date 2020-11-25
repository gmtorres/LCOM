#include "vector2d.h"

Vector v (double x, double y) {
  struct Vector v;
  v.x = x;
  v.y = y;
  return v;
}

void addVectors (struct Vector* v1, struct Vector* v2) {
  v1->x += v2->x;
  v1->y += v2->y;
}

void subVectors (struct Vector* v1, struct Vector* v2) {
  v1->x -= v2->x;
  v1->y -= v2->y;
}

void multVectors (struct Vector* v1, double a) {
  v1->x *= a;
  v1->y *= a;
}

double normaVector (struct Vector* v1) {
  return sqrt(v1->x * v1->x + v1->y * v1->y);
}

Vector normalVector(struct Vector* v1) {
  struct Vector v;
  v.x = -(v1->y);
  v.y = v1->x;
  return v;
}

double prodEscalar (struct Vector* v1, struct Vector* v2) {
  return (v1->x * v2->x) + (v1->y * v2->y);
}

// Projeçao de v2 em v1
Vector projVector (struct Vector* v1, struct Vector* v2) {
  struct Vector c = *v1;
  double d = normaVector(v1);
  d = prodEscalar(v1,v2)/(d*d);
  multVectors(&c,d);
  return c;
}

Vector multVectorMatriz (struct Vector* v1, double a, double b, double c, double d) {
  Vector vec;
  vec.x = (v1->x * a) + (v1->y * b);
  vec.y = (v1->x * c) + (v1->y * d);
  return vec;
}

Vector rotateVector (struct Vector *v1, double ang) {
  Vector rotV;
  rotV.x = (v1->x * cos(ang)) + (v1->y * sin(ang));
  rotV.y = (v1->x * (-sin(ang))) + (v1->y * cos(ang));
  return rotV;
}
