#pragma once


#include "quadtree.h"
#include "sprite.h"
#include "vector2d.h"
#include "v_gr.h"

#include "camera.h"



static SpriteBmp* tiles [91];


typedef struct Tile {
  int id;
  struct SpriteBmp *spr;
  int x;
  int y;
}Tile;

Tile tile (int i, uint16_t x, uint16_t ys);

void initiateTiles();
void freeTiles();
void drawTile (struct Tile* t);
void freeTile(struct Tile *t);


void drawTileCamera(struct Tile*t , struct Camera * cm);
