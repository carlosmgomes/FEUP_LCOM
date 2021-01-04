#ifndef __GRAPHICSMODE_H
#define __GRAPHICSMODE_H
#include "vge.h"
#include <lcom/lcf.h>
#include <machine/int86.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

/** @defgroup graphics graphics
 * @{
 *
 * Functions to deal with the graphics mode
 */

uint16_t XRes;
uint16_t YRes;
uint8_t bits_per_pixel;
vbe_mode_info_t vmi_p;
static char *video_mem; //    frame bugger VM ad
static char *double_buffer;


int vbe_int_10(uint16_t mode);

/** 
 * @brief initializes the graphics mode
 * @param mode the intended 16bit graphics mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *(vg_init)(uint16_t mode);

/**
 * @brief Returns information on the input VBE mode, including screen dimensions, color depth and VRAM physical address
 * 
 * Initializes unpacked vbe_mode__info_t structure passed as an address with
 *  the information of the input mode, by calling VBE function 0x01
 *  Return VBE Mode Information and unpacking the ModeInfoBlock struct
 *  returned by that function.
 * 
 * @param mode mode whose information should be returned
 * @param vmi_p address of vbe_mode_info_t structure to be initialized
 * @return 0 on success, non-zero otherwise
 */
int (vbe_mode_info)(uint16_t mode,vbe_mode_info_t *vmi_p);

/** 
 * @brief Draws a pixel on x, y with color provided
 * @param x horizontal coordinate
 * @param y vertical coordinate
 * @param color hexadecimal color to draw on x,y
 * @return 0 upon success, non-zero otherwise
*/
int(pixel_color)(uint16_t x, uint16_t y, uint32_t color);

/** 
 * @brief Draws a horizontal line with length 'len' and color 'color' using draw_pixel, starting on (x,y) 
 * @param x horizontal coordinate
 * @param y vertical coordinate
 * @param len length of the line
 * @param color hexadecimal color of the line
 * @return 0 upon success
*/
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/** 
 * @brief Draws a rectangle (using vg_draw_h_line) of color 'color', size (width * height) and starting on (x, y)
 * @param x horizontal coordinate
 * @param y vertical coordinate
 * @param width rectangle width
 * @param height rectangle height
 * @param color hexadecimal color of the rectangle
 * @return 0 upon success
*/
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/** 
 * @brief Draws a pattern using rectangles
 * @param mode intended graphics mode
 * @param no_rectangles number of rectangles
 * @param first first color
 * @param step color step (used to calculate next colors)
 * @return 0 upon success
*/
int(vg_draw_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);

/** 
 * @brief draws a xpm on the coords (x,y)
 * @param xpm_map_t map obtained from a xpm using xpm_load()
 * @param x horizontal starting coordinate
 * @param y vertical starting coordinate
 * @return 0 upon succes
 */
int(vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief switches from the helper buffer to the main one
 * 
 */
void double_buffer_update();

/**
 * @brief clears the double buffer
 * 
 */
void clean_double_buffer();
#endif /* __GRAPHICSMODE_H */
