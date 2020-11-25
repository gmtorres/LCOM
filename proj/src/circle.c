#include "circle.h"

Circle circle(Vector * v, double r){
  struct Circle c;
  c.pos = v;
  c.r = r;
  return c;
}
