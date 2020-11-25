#include "bitmap.h"


Bitmap* loadBitmap(const char* filename) {
    // allocating necessary size
    Bitmap* bmp = (Bitmap*) malloc(sizeof(Bitmap));

    // open filename in read binary mode
    FILE *filePtr;
    filePtr = fopen(filename, "rb");
    if (filePtr == NULL)
        return NULL;

    // read the bitmap file header
    BitmapFileHeader bitmapFileHeader;
    fread(&bitmapFileHeader, 2, 1, filePtr);

    // verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.type != 0x4D42) {
        fclose(filePtr);
        return NULL;
    }

    int rd;
    do {
        if ((rd = fread(&bitmapFileHeader.size, 4, 1, filePtr)) != 1)
            break;
        if ((rd = fread(&bitmapFileHeader.reserved, 4, 1, filePtr)) != 1)
            break;
        if ((rd = fread(&bitmapFileHeader.offset, 4, 1, filePtr)) != 1)
            break;
    } while (0);

    if (rd != 1) {
        fprintf(stderr, "Error reading file\n");
        //exit(-1);
    }

    // read the bitmap info header
    BitmapInfoHeader bitmapInfoHeader;
    fread(&bitmapInfoHeader, sizeof(BitmapInfoHeader), 1, filePtr);

    // move file pointer to the begining of bitmap data
    fseek(filePtr, bitmapFileHeader.offset, SEEK_SET);

    // allocate enough memory for the bitmap image data
     char* bitmapImage = ( char*) malloc(
            bitmapInfoHeader.imageSize);

    // verify memory allocation
    if (!bitmapImage) {
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    // read in the bitmap image data
    fread(bitmapImage, bitmapInfoHeader.imageSize, 1, filePtr);

    // make sure bitmap image data was read
    if (bitmapImage == NULL) {
        fclose(filePtr);
        return NULL;
    }

    // close file and return bitmap image data
    fclose(filePtr);

    bmp->bitmapData = bitmapImage;
    bmp->bitmapInfoHeader = bitmapInfoHeader;

    return bmp;
}

void freeBitmap(Bitmap * bmp){
  if(bmp == NULL)
    return;
  if(bmp->bitmapData == NULL)
    return;

  free(bmp->bitmapData);
  bmp->bitmapData = NULL;
  free(bmp);
  bmp = NULL;

}


void drawBitmap(Bitmap* bmp, int x, int y) {

    if (bmp == NULL)
        return;

    int width = bmp->bitmapInfoHeader.width;

    int height = bmp->bitmapInfoHeader.height;

    //char* bufferStartPos;
    char* imgStartPos;

    uint16_t hres = get_hres();
    //uint16_t vres = get_vres();

    int i, j;

    for (i = 0; i < height; i++) {
    	int posy = y + height - 1 - i;

      if(posy < 0)
        continue;
    	for (j = 0; j < width; j++) {
        int posx = x + j;
        if(posx < 0 || posx >= hres )
          continue;
    		//bufferStartPos = (char *)get_video_mem_buffer2() + (posx * 2) + (posy * get_hres() * 2);
        imgStartPos = bmp->bitmapData + j * 2 + i * width * 2;
        if(*(uint16_t*)imgStartPos != ALPHA)
            vg_paint_pixel(posx,posy,*imgStartPos , *(imgStartPos + 1));
    	}
    }
}

void drawBitmapPerct(Bitmap* bmp, int x, int y,double p){
  if(p<= 0 || p>1)
    return;
  if(x < -128)
    return;
  if (bmp == NULL)
      return;

  int width = bmp->bitmapInfoHeader.width;

  int height = bmp->bitmapInfoHeader.height;

  //char* bufferStartPos;
  char* imgStartPos;

  uint16_t hres = get_hres();
  uint16_t vres = get_vres();

  int i, j;

  for (i = 0; i < height*p; i++) {
    int posy = y + height - 1 - i;
    if(posy >= vres)
      break;
    if(posy < 0)
      continue;
    for (j = 0; j < width; j++) {
      int posx = x + j;
      if(posx < 0 || posx >= hres )
        continue;
      //bufferStartPos = (char *)get_video_mem_buffer2() + (posx * 2) + (posy * get_hres() * 2);
      imgStartPos = bmp->bitmapData + j * 2 + i * width * 2;
      if(*(uint16_t*)imgStartPos != ALPHA)
          vg_paint_pixel(posx,posy,*imgStartPos , *(imgStartPos + 1));
    }
  }
}

void drawBitmapFast(Bitmap * bmp , int x , int y){

  if (bmp == NULL)
      return;

  int width = bmp->bitmapInfoHeader.width;

  int height = bmp->bitmapInfoHeader.height;

  char* bufferStartPos = get_video_mem_buffer2();
  char* imgStartPos;

  uint16_t hres = get_hres();
  uint16_t vres = get_vres();

  int i, j = 0;

  if(x>= hres)
    return;

  if(x + width < hres && x >= 0 ){
    for (i = 0; i < height; i++) {
      int posy = y + height - 1 - i ;
      if(posy >= vres)
        continue;
      if(posy < 0)
        break;;
      imgStartPos = bmp->bitmapData + i * width * 2;
      char * buffer = (bufferStartPos + ( posy * hres + x ) *2);
      memcpy(buffer,imgStartPos , (width) * 2);
    }
  }else if(x>=0 && x + width >= hres){
    for (i = 0; i < height; i++) {
      int posy = y + height - 1 - i ;
      if(posy >= vres)
        continue;
      if(posy < 0)
        break;;
      imgStartPos = bmp->bitmapData + i * width * 2;
      char * buffer = (bufferStartPos + ( posy * hres + x ) *2);
      memcpy(buffer,imgStartPos , (hres - x )  * 2);
    }
  }else if(x < 0 && x + width < hres){
    for (i = 0; i < height; i++) {
      int posy = y + height - 1 - i ;
      if(posy >= vres)
        continue;
      if(posy < 0)
        break;;
      imgStartPos = bmp->bitmapData + (i * width  - x ) * 2;
      char * buffer = (bufferStartPos + ( posy * hres) *2);
      int v = width + x;
      if(v > 0)
      memcpy(buffer,imgStartPos ,   v * 2);
    }
  }
  else{

    for (i = 0; i < height; i++) {
      int posy = y + height - 1 - i;
      if(posy >= vres)
        continue;
      if(posy < 0)
        break;
      for (j = 0; j < width; j++) {
        int posx = x + j;
        if(posx < 0 || posx >= hres )
          continue;
        //bufferStartPos = (char *)get_video_mem_buffer2() + (posx * 2) + (posy * get_hres() * 2);
        imgStartPos = bmp->bitmapData + j * 2 + i * width * 2;
        if(*(uint16_t*)imgStartPos != ALPHA)
            vg_paint_pixel(posx,posy,*imgStartPos , *(imgStartPos + 1) );
        }
    }
  }
}


void drawBitmapAng(Bitmap* bmp, int x, int y, double ang) {

    if (bmp == NULL)
        return;


    int width = bmp->bitmapInfoHeader.width;

    int height = bmp->bitmapInfoHeader.height;

    int rx = x + width/2;
    int ry = y + height/2;

    //char* bufferStartPos;
    char* imgStartPos;

    //double tangente = tan(-ang/2);
    double cosseno = cos(ang);
    double seno = sin(ang);

    //char * buffer = (char *)get_video_mem_buffer2();


    int i, j;
    for (j = 0; j < height; j++) {
    	for (i = 0; i < width; i++) {

        imgStartPos = bmp->bitmapData + i * 2 + j * width * 2;
        if(*(uint16_t*)imgStartPos == ALPHA)
          continue;

        int posy = y + height - 1 - j;
        int posx = x + i;

        double dx = i + x - rx;
        double dy = y + height - 1 - j - ry;

        //struct Vector temp = v (dx, dy);
        //temp =  multVectorMatriz(&temp, 1, tangente, 0, 1);
        //temp = multVectorMatriz(&temp, 1, 0, seno, 1);
        //temp = multVectorMatriz(&temp, 1,tangente, 0, 1);
        struct Vector temp = v (dx, dy);
        temp = multVectorMatriz(&temp , cosseno , -seno  , seno , cosseno);

        posx = round(rx + temp.x);
        posy = round(ry + temp.y);
        if(posx < 0 || posx >= get_hres() || posy < 0 || posy >= get_vres())
          continue;
        vg_paint_pixel(posx,posy,*imgStartPos , *(imgStartPos + 1));

/*
        for(int a = 0 ; a < 2 ;a++){
          for(int b = -1 ; b <= 1; b+=2){
            Vector p = (a == 0) ? v(dx+b , dy) : v(dx , dy +b);
            if(p.x > width/2 || p.y > height/2)
              continue;
            p =  multVectorMatriz(&p , cosseno , -seno  , seno , cosseno);
            int posx2 = round(rx + p.x);
            int posy2 = round(ry + p.y);
            if(abs(posx - posx2) >=2 || abs(posy - posy2)>=2){
              if(posx2 < 0 || posx2 >= get_hres() || posy2 < 0 || posy2 >= get_vres())
                continue;
              imgStartPos = (a == 0) ? bmp->bitmapData + (i + b) + j * width * 2 : bmp->bitmapData + i * 2  + (j + b) * width;
              if(*(uint16_t*)imgStartPos != ALPHA)
                vg_paint_pixel((posx2 + posx)/2 ,(posy2 + posy)/2,*imgStartPos , *(imgStartPos + 1));
            }
          }
        }
*/

      }

    }
}


void drawBitmapFade(Bitmap * bmp, int x, int y , double ang, double min, double max){
  if (bmp == NULL)
      return;

  int d = max-min;

  int width = bmp->bitmapInfoHeader.width;

  int height = bmp->bitmapInfoHeader.height;

  int rx = x + width/2;
  int ry = y + height/2;

  //char* bufferStartPos;
  char* imgStartPos;

  //double tangente = tan(-ang/2);
  double cosseno = cos(ang);
  double seno = sin(ang);

  uint16_t * buffer = (uint16_t*)get_video_mem_buffer2();


  int i, j;
  for (j = 0; j < height; j++) {
    for (i = 0; i < width; i++) {

      imgStartPos = bmp->bitmapData + i * 2 + j * width * 2;
      if(*(uint16_t*)imgStartPos == ALPHA)
        continue;

      int posy = y + height - 1 - j;
      int posx = x + i;

      double dx = i + x - rx;
      double dy = y + height - 1 - j - ry;

      //struct Vector temp = v (dx, dy);
      //temp =  multVectorMatriz(&temp, 1, tangente, 0, 1);
      //temp = multVectorMatriz(&temp, 1, 0, seno, 1);
      //temp = multVectorMatriz(&temp, 1,tangente, 0, 1);
      struct Vector temp = v (dx, dy);
      temp = multVectorMatriz(&temp , cosseno , -seno  , seno , cosseno);

      posx = round(rx + temp.x);
      posy = round(ry + temp.y);
      if(posx < 0 || posx >= get_hres() || posy < 0 || posy >= get_vres())
        continue;
      //vg_paint_pixel(posx,posy,*imgStartPos , *(imgStartPos + 1));
      uint16_t *bf = buffer + (posx + get_hres() *posy);
      uint16_t color = *(uint16_t *)imgStartPos;
      uint16_t r2 = (color & 0x0000F800) >> 11;
      uint16_t g2 = (color & 0x000007E0) >> 5;
      uint16_t b2 = (color & 0x0000001F);
      int t = max - j * d/height;
      uint16_t c = *bf;
      *bf = ((((((c & 0xF800) >> 11) * (255-t) + r2 * t)/255)<<11) | (((((c & 0x07E0) >> 5) * (255-t) + g2 * t)/255)<<5) | (((c & 0x001F) * (255-t) + b2 * t)/255));

    }
  }
}
