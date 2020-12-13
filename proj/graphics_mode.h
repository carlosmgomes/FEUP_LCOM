#ifndef __GRAPHICSMODE_H
#define __GRAPHICSMODE_H
#include "vge.h"
#include <lcom/lcf.h>
#include <machine/int86.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

uint16_t XRes;
uint16_t YRes;
uint8_t bits_per_pixel;
vbe_mode_info_t vmi_p;
static char *video_mem; //    frame bugger VM ad
static char *double_buffer;


int vbe_int_10(uint16_t mode);
void *(vg_init)(uint16_t mode);
int (vbe_mode_info)(uint16_t mode,vbe_mode_info_t *vmi_p);
int(pixel_color)(uint16_t x, uint16_t y, uint32_t color);
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int(vg_draw_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);
int(vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);
#endif /* __GRAPHICSMODE_H */
