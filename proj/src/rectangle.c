#include "rectangle.h"

Rectangle rectangle(Vector * v , double comp , double larg, double ang){
  struct Rectangle r;
  r.pos = v;
  r.width = comp;
  r.height = larg;
  r.ang = ang;
  return r;
}
