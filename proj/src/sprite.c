#include "sprite.h"

Sprite * sInit () {
  struct Sprite * sp = (Sprite *) malloc(sizeof(Sprite));
  sp->width = 0;
  sp->height = 0;
  sp->map = NULL;
  return sp;
}


void addImage(struct Sprite * spr, const char *filename[]){

  spr->map = read_xpm(filename , &spr->width , &spr->height);
  if(spr->map == NULL){
    printf("Failed to read xpm.\n");
  }

}

void drawSprite(struct Sprite * spr , uint16_t x, uint16_t y ){
  vg_draw_bitmap(spr->map , spr->width , spr->height , x , y);
}

void drawSpriteAng(struct Sprite *spr, uint16_t x, uint16_t y , double ang){
  vg_draw_bitmap_rotation(spr->map , spr->width, spr->height, x , y , x + spr->width/2, y + spr->height/2 , ang);
}


SpriteBmp *sBmpInit(const char *filename){
  struct SpriteBmp * b =(SpriteBmp *) malloc(sizeof(SpriteBmp));
  addImageBmp(b , filename);
  b->width = &b->bmp->bitmapInfoHeader.width;
  b->height = &b->bmp->bitmapInfoHeader.height;
  return b;
}

void freeBmp(SpriteBmp *sbmp){
  if(sbmp == NULL)
    return;
  freeBitmap(sbmp->bmp);
  free(sbmp);
  sbmp = NULL;
}

void addImageBmp(struct SpriteBmp *sp, const char *filename){
  sp->bmp = loadBitmap(filename);
  sp->width = &sp->bmp->bitmapInfoHeader.width;
  sp->height = &sp->bmp->bitmapInfoHeader.height;

}


void drawSpriteBmp(struct SpriteBmp *spr, int x, int y){
  //vg_draw_bitmap(sp->bmp->bitmapData , sp->bmp->bitmapInfoHeader.width , sp->bmp->bitmapInfoHeader.height , x , y);
  //void * mem = get_video_mem();
  //drawBitmap(sp->bmp,x,y);
  drawBitmap(spr->bmp , x , y);
}

void drawSpriteBmpAng(struct SpriteBmp *spr, int x, int y , double ang){
  drawBitmapAng(spr->bmp , x , y , ang);
}
