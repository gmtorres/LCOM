#pragma once

#include "vector2d.h"
#include <stdbool.h>
#include <lcom/lcf.h>
#include "sprite.h"
#include "v_gr.h"

static uint8_t mouse_byte_read;

//typedef enum {LB_PRESSED, LB_RELEASED, RB_PRESSED, RB_RELEASED, BUTTON_EV, MOUSE_MOV , NOTHING} mouse_evt;


typedef struct Mouse {
  enum {LB_PRESSED, LB_RELEASED, RB_PRESSED, RB_RELEASED, BUTTON_EV, MOUSE_MOV , NOTHING} event;
  SpriteBmp * sp;
  struct Vector pos;
} Mouse;


Mouse msInit();

void setImage(struct Mouse *m,SpriteBmp *sp);

void drawMouse(struct Mouse *m);

void packetUpdate(struct packet *pp, struct Mouse *m);
