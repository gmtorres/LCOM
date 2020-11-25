#pragma once

#include "vector2d.h"
#include "v_gr.h"
#include <stdbool.h>

typedef struct Camera{

  Vector * translacao;

  Vector alinhamento;

  bool borders;
  Vector border_min;
  Vector border_max;

  bool ahead;
  Vector *speed;
  Vector v_ahead;

}Camera;

Camera cameraInit(struct Vector* v, Vector al , bool b, bool ahead);
void setBorders(Camera * cm,Vector min, Vector max);
void setSpeed (Camera * cm , Vector *speed);
Vector cameraDeslocamento(struct Camera * cm);
