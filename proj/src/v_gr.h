#include <lcom/vbe.h>
#include <math.h>

#define LINEAR_FRAMEBUFFER 1<<14
#define VBE_INT 0x10

#define FUNCTION_GET_VBE_CNTRL_INFO 0x4F00
#define FUNCTION_GET_VBE_MODE_INFO 0x4F01
#define FUNCTION_SET_VBE_MODE 0x4F02
#define FUNCTION_GET_VBE_MODE 0x4F03


int vbe_mode_info(uint16_t mode, vbe_mode_info_t * vmi);
int getVBEMode(void);

int vbe_set_mode(uint16_t mode);

int vg_set_text() ;

void *get_video_mem(void);

void *get_video_mem_buffer2(void);

uint16_t get_vres(void);
uint16_t get_hres(void);
uint16_t get_bitsPerPixels(void);

void drawCircle(uint16_t x, uint16_t y, uint16_t r, uint32_t color);

int vg_color_pixel(uint16_t x, uint16_t y, uint32_t color);

void vg_paint_pixel(uint16_t x, uint16_t y, char cor1, char cor2);

void background(uint32_t color);

int vg_show();

int directColorMode(void);  //retorna se esta em direct color mode

int vg_draw_bitmap(char * bmp , uint16_t w, uint16_t h, uint16_t x , uint16_t y );

int vbe_get_cntrl_info(vg_vbe_contr_info_t * cntr);

int vg_draw_bitmap_rotation( char *bmp , uint16_t w, uint16_t h, uint16_t x , uint16_t y, uint16_t rx, uint16_t ry, double ang);

void applyMask(uint32_t color, uint8_t t);
