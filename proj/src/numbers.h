#pragma once


#include "sprite.h"
#include <stdbool.h>

#define PADDING 10
#define TICKS 60


static SpriteBmp* big_numbers[10];
static SpriteBmp* small_numbers[11];
static SpriteBmp* tiny_numbers[11];

void initiateNumbers();
void freeNumbers();

void draw_digit(uint8_t n , double x, double y, bool small);

void draw_big_number(uint32_t n , double x, double y);

void draw_time_ticks(uint32_t ticks , double x, double y);

void draw_slash(double x, double y);

void setPadding(int n);

void draw_hour(double x, double y, uint8_t hour , uint8_t min, uint8_t sec );

void draw_ending_time(uint32_t ticks , double x, double y);
