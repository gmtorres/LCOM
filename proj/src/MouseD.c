#include "MouseD.h"

Mouse msInit() {
  Mouse ms;
  ms.event = NOTHING;
  ms.pos.x = get_hres()/2;
  ms.pos.y = get_vres()/2;
  ms.sp=NULL;
  return ms;
}

void setImage(struct Mouse *m,SpriteBmp *sp){
  m->sp = sp;
}

void drawMouse(struct Mouse *m){
  drawSpriteBmp(m->sp,m->pos.x,m->pos.y);
}

bool leftButtonPressed(struct packet pp) {
  return (pp.lb && !pp.rb && !pp.mb);
}
bool rightButtonPressed(struct packet pp) {
  return (!pp.lb && pp.rb && !pp.mb);
}
bool noButtonPressed(struct packet pp) {
  return (!pp.lb && !pp.rb && !pp.mb);
}

static double max(double a, double b){
  return (a > b) ? a : b;
}
static double min(double a, double b){
  return (a < b) ? a : b;
}

void packetUpdate(struct packet *pp, struct Mouse *m) {
  m->pos.x = min(get_hres() - *m->sp->width,max(m->pos.x + pp->delta_x,0));
  m->pos.y = min(get_vres() - *m->sp->height,max(m->pos.y - pp->delta_y,0));

  if(pp->delta_x != 0 || pp->delta_y != 0){
    m->event = MOUSE_MOV;
  }

  if (leftButtonPressed(*pp)) {
    m->event = LB_PRESSED;
  }
  else if(rightButtonPressed(*pp))
    m->event = RB_PRESSED;
  else if(noButtonPressed(*pp)){
    if(m->event == RB_PRESSED)
      m->event = RB_RELEASED;
    else if(m->event == LB_PRESSED)
      m->event = LB_RELEASED;
  }else if (!noButtonPressed(*pp))
    m->event = BUTTON_EV;
}
