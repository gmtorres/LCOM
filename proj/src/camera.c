#include "camera.h"

Camera cameraInit(struct Vector* vec, Vector al , bool b , bool ahead){
  struct Camera cm;
  cm.translacao = vec;
  cm.alinhamento = al;
  cm.borders = b;
  cm.ahead = ahead;
  cm.speed = NULL;
  cm.v_ahead = v(0,0);
  cm.border_min = v(0,0);
  cm.border_max = v(0,0);
  return cm;
}
void setBorders(Camera * cm,Vector min, Vector max){
  cm->border_min = min;
  cm->border_max = max;
}
void setSpeed (Camera * cm , Vector *speed){
  cm->speed = speed;
}


static double max(double a, double b){
  return (a > b) ? a : b;
}
static double min(double a, double b){
  return (a < b) ? a : b;
}

Vector cameraDeslocamento(struct Camera * camera){
  struct Vector temp = *camera->translacao;
  subVectors(&temp,&camera->alinhamento);

  if(camera->ahead && camera->speed != NULL){
    camera->v_ahead.x = camera->v_ahead.x - 0.0001 * camera->v_ahead.x + 0.002*camera->speed->x;
    camera->v_ahead.y = camera->v_ahead.y - 0.0001 * camera->v_ahead.y + 0.002*camera->speed->y;
    addVectors(&temp,&camera->v_ahead);
  }

  if(camera->borders){
    temp.x = round(min(max(camera->border_min.x , temp.x) , camera->border_max.x - get_hres()));
    temp.y = round(min(max(camera->border_min.y , temp.y) , camera->border_max.y - get_vres()));
  }
  return temp;
}
