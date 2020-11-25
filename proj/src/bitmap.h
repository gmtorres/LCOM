#pragma once


/** @defgroup Bitmap Bitmap
 * @{
 * Functions for manipulating bitmaps - based on code provided by Henrique Ferrolho
 */

 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 #include "v_gr.h"
 #include <math.h>

 #include "vector2d.h"

 #define ALPHA 0xFFFF

typedef enum {
    ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT
} Alignment;

typedef struct {
    unsigned short type; // specifies the file type
    unsigned int size; // specifies the size in bytes of the bitmap file
    unsigned int reserved; // reserved; must be 0
    unsigned int offset; // specifies the offset in bytes from the bitmapfileheader to the bitmap bits
} __attribute__((packed)) BitmapFileHeader;

typedef struct {
    unsigned int size; // specifies the number of bytes required by the struct
    int width; // specifies width in pixels
    int height; // specifies height in pixels
    unsigned short planes; // specifies the number of color planes, must be 1
    unsigned short bits; // specifies the number of bit per pixel
    unsigned int compression; // specifies the type of compression
    unsigned int imageSize; // size of image in bytes
    int xResolution; // number of pixels per meter in x axis
    int yResolution; // number of pixels per meter in y axis
    unsigned int nColors; // number of colors used by the bitmap
    unsigned int importantColors; // number of colors that are important
} __attribute__((packed)) BitmapInfoHeader;

typedef struct {
    BitmapInfoHeader bitmapInfoHeader;
    char* bitmapData;
} Bitmap;

/**
 * @brief Loads a bmp image - code provided by Henrique Ferrolho
 *
 * @param filename Path of the image to load
 * @return Non NULL pointer to the image buffer
 */
Bitmap* loadBitmap(const char* filename);

void freeBitmap(Bitmap * bmp);

void drawBitmap(Bitmap* bmp, int x, int y);

void drawBitmapPerct(Bitmap* bmp, int x, int y,double p);

void drawBitmapAng(Bitmap* bmp, int x, int y, double ang);

void drawBitmapFast(Bitmap * bmp , int x , int y); // do not takes into account the ALPHA and is better for bmp that dont go accross the border and are squares, no angle

void drawBitmapFade(Bitmap * bmp, int x, int y , double ang, double min, double max);
