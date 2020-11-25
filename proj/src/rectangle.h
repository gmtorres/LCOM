#pragma once

#include "vector2d.h"

typedef struct Rectangle{
  Vector * pos;
  double width;
  double height;
  double ang;
}Rectangle;

Rectangle rectangle(Vector * v , double comp , double larg, double ang);
