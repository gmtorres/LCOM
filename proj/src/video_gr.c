
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "v_gr.h"
#include <sys/mman.h>
#include <lcom/video_gr.h>
#include <liblm.h>
#include <lcom/vbe.h>
#include <machine/int86.h>
#include <stdint.h>
#include <stdio.h>
#include "vector2d.h"



static char *video_mem; /* Process (virtual) address to which VRAM is mapped */

static uint16_t hres; /*XResolution*/
static uint16_t vres; /*YResolution*/
static uint16_t bits_per_pixel; /*Bits Per Pixel*/

static char* buffer2;

void * vg_init(uint16_t mode){

  vbe_mode_info_t v_mode_info;

  if(vbe_mode_info(mode,&v_mode_info) != 0)
    return NULL;

  if(vbe_set_mode(mode) != 0)
    return NULL;

  hres = v_mode_info.XResolution;
  vres = v_mode_info.YResolution;
  bits_per_pixel = v_mode_info.BitsPerPixel;

  int r;
  struct minix_mem_range mr; /*physical memory range*/
  unsigned int vram_base = v_mode_info.PhysBasePtr;  /*VRAM’s physical addresss*/
  unsigned int vram_size  = (hres * vres * bits_per_pixel) / 8;  /*VRAM’s size, but you can usethe frame-buffer size, instead*/

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);/*Map memory*/

	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
	if(video_mem == MAP_FAILED)
		panic("couldn't map video memory");

	if (video_mem == NULL){
			printf("Error on vg_init.\n");
			vg_exit();
			return NULL;
	}

  buffer2 = malloc( bits_per_pixel/8 + hres * vres * bits_per_pixel / 8);


	return video_mem;
}

/* Set default text mode */
int vg_set_text() {
	struct reg86u reg86;

	memset(&reg86, 0, sizeof(reg86)); /* zero the structure */

	reg86.u.b.intno = 0x10; /* BIOS video services */
	reg86.u.b.ah = 0x00; /* Set Video Mode function */
	reg86.u.b.al = 0x03; /* 80x25 text mode*/
	if( sys_int86(&reg86) != OK ) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	}
  free(buffer2);
	return 0;
}


int vbe_set_mode(uint16_t mode) {

	struct reg86u r;

	memset(&r,0, sizeof(r));

	r.u.w.ax = FUNCTION_SET_VBE_MODE; // VBE call, function 02 -- set VBE mode //0x4F02
	r.u.w.bx = LINEAR_FRAMEBUFFER  |  mode; // set bit 14: linear framebuffer  //0x105
	r.u.b.intno = VBE_INT; // interrupt VBE (0x10)

	if( sys_int86(&r) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return 1;
	}
  switch(r.u.b.ah){
    case 0x00:
      return 0;
    case 0x01:
      printf("Function call failed!\n");
      return 1;
    case 0x02:
      printf("Function is not supported in current HW configuration!\n");
      return 2;
    case 0x03:
      printf("Function is invalid in current video mode!\n");
      return 3;
    default:
      return 4;
  }
	return 0;
}

int vbe_mode_info(uint16_t mode, vbe_mode_info_t * vmi){
	struct reg86u r;
	memset(&r,0, sizeof(r));

	mmap_t map;
	memset(&map,0,sizeof(map));

	lm_init(false);

	if (lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL){ //alloc memory needed
		printf("Bad allocation of memory!\n");
		return 1;
	}

	r.u.w.ax = FUNCTION_GET_VBE_MODE_INFO; // VBE call, function 02 -- set VBE mode //0x4F02
	r.u.w.cx = mode; // set bit 14: linear framebuffer  //0x105
	r.u.w.es = PB2BASE(map.phys);
	r.u.w.di = PB2OFF(map.phys);
	r.u.b.intno = VBE_INT; // interrupt VBE (0x10)

	if( sys_int86(&r) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		lm_free(&map);
		return 1;
	}
  switch(r.u.b.ah){
		case 0x00:break;
    case 0x01:
      printf("Function call failed!\n");
			lm_free(&map);
      return 1;
    case 0x02:
      printf("Function is not supported in current HW configuration!\n");
			lm_free(&map);
      return 2;
    case 0x03:
      printf("Function is invalid in current video mode!\n");
			lm_free(&map);
      return 3;
    default:
			lm_free(&map);
      return 4;
  }
	memcpy(vmi, map.virt, map.size);
	lm_free(&map);
	return 0;
}

int getVBEMode(void){
    struct reg86u r;
    r.u.w.ax = 0x4F03;
		r.u.b.intno = VBE_INT;
		if( sys_int86(&r) != OK ) {
			printf("getVBEMode: sys_int86() failed \n");
			return 1;
		}
    return r.u.w.bx;
}

int directColorMode(void){
  return getVBEMode() > 0x107;
}

int vg_show(){
  memcpy(video_mem,buffer2 , hres * vres * bits_per_pixel / 8);
  return 0;

}

int vg_color_pixel(uint16_t x, uint16_t y, uint32_t color) {
  if (x > 0 && x < hres && y > 0 && y < vres)
    *(buffer2 + (x  + y* hres) * (bits_per_pixel)/8) = (uint32_t)color;
	return 0;
}

int vg_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
	for (unsigned int j = 0; j < len && x+ j < hres; ++j) {
		vg_color_pixel(x+j,y,color);
	}
	return 0;
}

void vg_paint_pixel(uint16_t x, uint16_t y, char cor1, char cor2){
  if (x > 0 && x < hres && y > 0 && y < vres){
    *(buffer2+ (x  + y* hres) * (bits_per_pixel)/8) = cor1;
    *(buffer2+ (x  + y* hres) * (bits_per_pixel)/8 + 1) = cor2;
  }
}




int vg_draw_rectangle	(uint16_t x, uint16_t	y, uint16_t width, uint16_t	height, uint32_t color) {
	for (unsigned int i = 0; i < height && y + i < vres; ++i) {
		vg_draw_hline(x,y+i,width,color);
	}
	return 0;
}


void *get_video_mem(void){
	return video_mem;
}
void *get_video_mem_buffer2(void){
  return buffer2;
}
uint16_t get_hres(void){
	return hres;
}
uint16_t get_vres(void){
	return vres;
}
uint16_t get_bitsPerPixels(void){
	return bits_per_pixel;
}


void drawCircle(uint16_t x, uint16_t y, uint16_t r, uint32_t color) {
	for (unsigned int i = x-r; i <= x+r; i++) {
		for (unsigned int j = y-r; j <= y+r; j++) {
			if ((i-x)* (i-x) + (j-y) * (j-y) <= r*r) {
				vg_color_pixel(i,j,color);
			}
		}
	}
}

void background (uint32_t color) {
			vg_draw_rectangle(0,0,hres,vres,color);
}

int vg_draw_bitmap( char *bmp , uint16_t w, uint16_t h, uint16_t x , uint16_t y ){


    for(uint16_t j = 0; j < h ; ++j){
      for(uint16_t i = 0; i < w ;++i){
        vg_color_pixel(i+x,j+y ,bmp[w * j + i]);
      }
    }

    return 1;
}

int vg_draw_bitmap_rotation( char *bmp , uint16_t w, uint16_t h, uint16_t x , uint16_t y, uint16_t rx, uint16_t ry, double ang){

  double tangente = -tan(ang/2);
  double seno = sin(ang);

  for(uint16_t j = 0; j < h ; ++ j){
    for(uint16_t i = 0; i < w ;++i){
      double dx = i + x - rx;
      double dy = j + y - ry;
      struct Vector temp = v (dx, dy);
      temp =  multVectorMatriz(&temp, 1, tangente, 0, 1);
      temp = multVectorMatriz(&temp, 1, 0, seno, 1);
      temp = multVectorMatriz(&temp, 1,tangente, 0, 1);
      vg_color_pixel(rx + temp.x , ry + temp.y ,bmp[w * j + i] );
    }
  }
  return 1;
}

void applyMask(uint32_t color, uint8_t t){
  //uint32_t * bf = (uint32_t *)(buffer2 + (x  + y* hres) * (bits_per_pixel)/8);
  uint16_t *bf = (uint16_t *)buffer2;
  uint16_t r2 = (color & 0x0000F800) >> 11;
  uint16_t g2 = (color & 0x000007E0) >> 5;
  uint16_t b2 = (color & 0x0000001F);
  uint16_t xmax = get_hres();
  uint16_t ymax = get_vres();
  for(int y = 0 ; y < ymax ; y++){
    for(int x = 0;x < xmax;x++,bf++){
      uint16_t c = *bf;
      *bf = ((((((c & 0xF800) >> 11) * (255-t) + r2 * t)/255)<<11) | (((((c & 0x07E0) >> 5) * (255-t) + g2 * t)/255)<<5) | (((c & 0x001F) * (255-t) + b2 * t)/255));
    }
  }

}



int vbe_get_cntrl_info(vg_vbe_contr_info_t * cntr){

  struct reg86u r;
	memset(&r,0, sizeof(r));

  //vg_vbe_contr_info_t temp;

	mmap_t map;
	memset(&map,0,sizeof(map));

  struct VBEInfoBlock{
    uint8_t VbeSignature[4];
    uint8_t VbeVersion[2];
    uint32_t * OemStringPtr;
    uint8_t Capabilities[4];
    uint32_t * VideoModePtr;
    uint16_t TotalMemory;

    uint16_t OemSoftwareRev;
    uint32_t  * OemVendorNamePtr;
    uint32_t  * OemProductNamePtr;
    uint32_t  * OemProductRevPtr;
    uint8_t Reserved[222];
    uint8_t OemData[256];
  } vbeinfo;

  memcpy(vbeinfo.VbeSignature,"VESA", sizeof(vbeinfo.VbeSignature));
  uint16_t v = 0x0200;
  memcpy(vbeinfo.VbeVersion,&v, sizeof(vbeinfo.VbeVersion));

  lm_init(true);

	if (lm_alloc(sizeof(vbeinfo), &map) == NULL){ //alloc memory needed
		printf("Bad allocation of memory!\n");
		return 1;
	}


	r.u.w.ax = FUNCTION_GET_VBE_CNTRL_INFO; // VBE call, function 02 -- set VBE mode //0x4F02
	r.u.w.es = PB2BASE(map.phys);
	r.u.w.di = PB2OFF(map.phys);
	r.u.b.intno = VBE_INT; // interrupt VBE (0x10)

  printf("%lx \t %x \t %x\n", map.phys , r.u.w.es, r.u.w.di );

	if( sys_int86(&r) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		lm_free(&map);
		return 1;
	}
  switch(r.u.b.ah){
		case 0x00:break;
    case 0x01:
      printf("Function call failed!\n");
			lm_free(&map);
      return 1;
    case 0x02:
      printf("Function is not supported in current HW configuration!\n");
			lm_free(&map);
      return 2;
    case 0x03:
      printf("Function is invalid in current video mode!\n");
			lm_free(&map);
      return 3;
    default:
			lm_free(&map);
      return 4;
  }

  printf("%x\n", (uint32_t)&vbeinfo.VbeSignature);
  printf("%x\n", (uint32_t)&vbeinfo.VbeVersion);
  printf("%p\n", vbeinfo.OemStringPtr);
  printf("%p\n", map.virt);

  void * farptr;

  memcpy (cntr->VBESignature , vbeinfo.VbeSignature , sizeof(vbeinfo.VbeSignature));
  memcpy (cntr->VBEVersion , vbeinfo.VbeVersion , sizeof(vbeinfo.VbeVersion));

  farptr = (void *)(((uint32_t)vbeinfo.OemStringPtr & 0xFFFF0000)>>12);
  farptr = (void *)((uint32_t)farptr + (uint32_t)PB2OFF((uint32_t)vbeinfo.OemStringPtr));
  farptr = (void*)((uint32_t)farptr + ((uint32_t)map.virt));
  cntr->OEMString = farptr;

  farptr = (void *)(((uint32_t)vbeinfo.VideoModePtr & 0xFFFF0000)>>12);
  farptr = (void *)((uint32_t)farptr + (uint32_t)PB2OFF((uint32_t)vbeinfo.VideoModePtr));
  farptr = (void*)((uint32_t)farptr + ((uint32_t)map.virt));
  cntr->VideoModeList = farptr;

  cntr->TotalMemory = vbeinfo.TotalMemory/1024;

  farptr = (void *)(((uint32_t)vbeinfo.OemVendorNamePtr & 0xFFFF0000)>>12);
  farptr = (void *)((uint32_t)farptr + (uint32_t)PB2OFF((uint32_t)vbeinfo.OemVendorNamePtr));
  farptr = (void*)((uint32_t)farptr + ((uint32_t)map.virt));
  cntr->OEMVendorNamePtr = farptr;

  farptr = (void *)(((uint32_t)vbeinfo.OemProductNamePtr & 0xFFFF0000)>>12);
  farptr = (void *)((uint32_t)farptr + (uint32_t)PB2OFF((uint32_t)vbeinfo.OemProductNamePtr));
  farptr = (void*)((uint32_t)farptr + ((uint32_t)map.virt));
  cntr->OEMProductNamePtr = farptr;

  farptr = (void *)(((uint32_t)vbeinfo.OemProductRevPtr & 0xFFFF0000)>>12);
  farptr = (void *)((uint32_t)farptr + (uint32_t)PB2OFF((uint32_t)vbeinfo.OemProductRevPtr));
  farptr = (void*)((uint32_t)farptr + ((uint32_t)map.virt));
  cntr->OEMProductRevPtr = farptr;

  lm_free(&map);

  return 0;
}
