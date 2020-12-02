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

int vbe_int_10(uint16_t mode);
void*(vg_init)(uint16_t	mode);

 	
#endif /* __GRAPHICSMODE_H */
