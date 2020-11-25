#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <lcom/read_xpm.h>
#include "v_gr.h"

#include "bitmap.h"


typedef struct Sprite {
  int width;
  int height;
  char * map;
  //int num_figuras;
  //int figura_atual;

}Sprite;

Sprite *sInit ();

void addImage(struct Sprite *sp,const char *filename []);

void drawSprite(struct Sprite *sp, uint16_t x, uint16_t y);

void drawSpriteAng(struct Sprite *sp, uint16_t x, uint16_t y , double ang);

//char * read_xpm_direct_mode(char * filename , uint16_t *width , uint16_t *height);

typedef struct SpriteBmp {
  Bitmap *bmp;
  int *width;
  int *height;

}SpriteBmp;

SpriteBmp * sBmpInit(const char *filename);

void freeBmp(SpriteBmp *sbmp);

void addImageBmp(struct SpriteBmp *sp, const char *filename);

void drawSpriteBmp(struct SpriteBmp *sp, int x, int y);

void drawSpriteBmpAng(struct SpriteBmp *sp, int x, int y , double ang);
