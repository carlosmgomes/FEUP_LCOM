#ifndef __GRAPHICSMODE_H
#define __GRAPHICSMODE_H
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "vge.h"
#include <machine/int86.h>



uint16_t XRes;
uint16_t YRes;
uint8_t  bits_per_pixel;
vbe_mode_info_t vmi_p;
static char *video_mem; 

int vbe_int_10(uint16_t mode);
void*(vg_init)(uint16_t	mode);
int draw_pixel(uint16_t x, uint16_t y, uint32_t color);
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int(vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int draw_pixmap(uint8_t *map, xpm_image_t img, uint16_t x, uint16_t y);

 	
#endif /* __GRAPHICSMODE_H */
