#include "numbers.h"

static int padding = 10;

void setPadding(int n){
  padding = n;
}

void initiateNumbers(){
  memset(big_numbers , 0 ,sizeof(big_numbers));
  big_numbers[0] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_big/numbers_big_zero.bmp");//a
  big_numbers[1] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_big/numbers_big_one.bmp");//a
  big_numbers[2] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_big/numbers_big_two.bmp");//a
  big_numbers[3] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_big/numbers_big_three.bmp");//a
  big_numbers[4] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_big/numbers_big_four.bmp");//a
  big_numbers[5] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_big/numbers_big_five.bmp");//a
  big_numbers[6] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_big/numbers_big_six.bmp");//a
  big_numbers[7] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_big/numbers_big_seven.bmp");//a
  big_numbers[8] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_big/numbers_big_eight.bmp");//a
  big_numbers[9] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_big/numbers_big_nine.bmp");//a

  memset(small_numbers , 0 ,sizeof(small_numbers));
  small_numbers[0] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_small/zero.bmp");//a
  small_numbers[1] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_small/one.bmp");//a
  small_numbers[2] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_small/two.bmp");//a
  small_numbers[3] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_small/three.bmp");//a
  small_numbers[4] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_small/four.bmp");//a
  small_numbers[5] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_small/five.bmp");//a
  small_numbers[6] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_small/six.bmp");//a
  small_numbers[7] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_small/seven.bmp");//a
  small_numbers[8] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_small/eight.bmp");//a
  small_numbers[9] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_small/nine.bmp");//a
  small_numbers[10] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_small/slash.bmp");//a

  memset(tiny_numbers , 0 ,sizeof(tiny_numbers));
  tiny_numbers[0] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_tiny/zero.bmp");//a
  tiny_numbers[1] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_tiny/one.bmp");//a
  tiny_numbers[2] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_tiny/two.bmp");//a
  tiny_numbers[3] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_tiny/three.bmp");//a
  tiny_numbers[4] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_tiny/four.bmp");//a
  tiny_numbers[5] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_tiny/five.bmp");//a
  tiny_numbers[6] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_tiny/six.bmp");//a
  tiny_numbers[7] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_tiny/seven.bmp");//a
  tiny_numbers[8] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_tiny/eight.bmp");//a
  tiny_numbers[9] = sBmpInit("/home/lcom/labs/proj/resources/images/numbers_tiny/nine.bmp");//a

}
void freeNumbers(){
  for(unsigned int i = 0; i < sizeof(big_numbers) / sizeof(big_numbers[0]);++i)
    freeBmp(big_numbers[i]);
  for(unsigned int i = 0; i < sizeof(small_numbers) / sizeof(small_numbers[0]);++i)
    freeBmp(small_numbers[i]);
  for(unsigned int i = 0; i < sizeof(tiny_numbers) / sizeof(tiny_numbers[0]);++i)
    freeBmp(tiny_numbers[i]);
}

void draw_digit(uint8_t n , double x, double y,bool small){
  if(n>9)
    return;
  if(small)
    drawBitmap(small_numbers[n]->bmp,x,y);
  else
    drawBitmap(big_numbers[n]->bmp,x,y);
}

void draw_big_number(uint32_t n , double x, double y){

  if(n == 0){
    draw_digit(0,x,y,1);
  }

  unsigned int divi = 1;
  for(;divi <= n/10;divi*=10);

  while(n){
    draw_digit((n/divi),x,y,1);
    x+= padding + *(big_numbers[0]->width);

    if(n%divi == 0 && n>=10)
      draw_digit(0,x,y,1);

    n %= divi;


    divi/=10;
  }


}

void draw_time_ticks(uint32_t ticks , double x, double y){

  uint8_t ms = ticks % TICKS * (10.0/6);
  uint8_t Mms = ms/10;
  ms %=10;
  uint32_t s = (ticks/TICKS)%60;
  uint32_t m = ticks/(TICKS*60)%100;
  uint8_t Ms = s/10;
  s %=10;
  uint32_t Mm = m/10;
  m %=10;
  draw_digit(Mm,x,y,1); x += padding + *(big_numbers[0]->width);
  draw_digit(m,x,y,1);  x += padding + *(big_numbers[0]->width);x += padding + *(big_numbers[0]->width);
  draw_digit(Ms,x,y,1); x += padding + *(big_numbers[0]->width);
  draw_digit(s,x,y,1);  x += padding + *(big_numbers[0]->width);x += padding + *(big_numbers[0]->width);
  draw_digit(Mms,x,y,1); x += padding + *(big_numbers[0]->width);
  draw_digit(ms,x,y,1);  x += padding + *(big_numbers[0]->width);x += padding + *(big_numbers[0]->width);

}
void draw_slash(double x, double y){
  drawBitmap(small_numbers[10]->bmp,x,y);
}

void draw_hour(double x, double y, uint8_t hour , uint8_t min, uint8_t sec){
  uint8_t h = hour%10;
  uint8_t Mh = hour/10;
  uint8_t m = min%10;
  uint8_t Mm = min/10;
  uint8_t s = sec%10;
  uint8_t Ms = sec/10;
  draw_digit(Mh,x,y,1); x += padding + *(big_numbers[0]->width);
  draw_digit(h,x,y,1);  x += padding + *(big_numbers[0]->width);x += padding + *(big_numbers[0]->width);
  draw_digit(Mm,x,y,1); x += padding + *(big_numbers[0]->width);
  draw_digit(m,x,y,1);  x += padding + *(big_numbers[0]->width);x += padding + *(big_numbers[0]->width);
  draw_digit(Ms,x,y,1); x += padding + *(big_numbers[0]->width);
  draw_digit(s,x,y,1);  x += padding + *(big_numbers[0]->width);x += padding + *(big_numbers[0]->width);

}

void draw_ending_time(uint32_t ticks , double x, double y){

  uint8_t ms = ticks % TICKS * (10.0/6);
  uint8_t Mms = ms/10;
  ms %=10;
  uint32_t s = (ticks/TICKS)%60;
  uint32_t m = ticks/(TICKS*60)%100;
  uint8_t Ms = s/10;
  s %=10;
  uint32_t Mm = m/10;
  m %=10;
  draw_digit(Mm,x,y,1); x += padding + *(tiny_numbers[0]->width);
  draw_digit(m,x,y,1);  x += padding + *(tiny_numbers[0]->width);x += padding + *(tiny_numbers[0]->width);
  draw_digit(Ms,x,y,1); x += padding + *(tiny_numbers[0]->width);
  draw_digit(s,x,y,1);  x += padding + *(tiny_numbers[0]->width);x += padding + *(tiny_numbers[0]->width);
  draw_digit(Mms,x,y,1); x += padding + *(tiny_numbers[0]->width);
  draw_digit(ms,x,y,1);  x += padding + *(tiny_numbers[0]->width);x += padding + *(tiny_numbers[0]->width);

}
