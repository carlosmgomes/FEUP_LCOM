#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include "graphics_mode.h"
#include "sprite.h"
#include "xpm.h"
#include <stdbool.h>
#include <stdint.h>

extern uint8_t bits_per_pixel;

Disc *create_disc(const char *circle[]) {
  //allocate space for the "disc"
  Disc *d = (Disc *) malloc(sizeof(Disc));
  d->map = xpm_load(circle, XPM_8_8_8, &(d->img));
  d->x = XRes * 0.15;
  d->y = 5;
  return d;
}

int draw_disc(Disc *d) {
  uint8_t *map = d->map;
  uint32_t color;
  unsigned bpp = bits_per_pixel / 8;

  for (int y = 0; y < d->img.height; y++) {
    for (int x = 0; x < d->img.width; x++) {
      color = 0;
      for (unsigned b = 0; b < bpp; b++) {
        color += *(map + b) << b * 8;
      }
      if (color != xpm_transparency_color(XPM_8_8_8)) {
        pixel_color(d->x + x, d->y + y, color);
        map += bpp;
      }
    }
  }
  return 0;
}

int delete_disc(Disc *d) {
  free(d);
  return 0;
}

void move_disc_right(Disc *d) {
  if (d->x <= XRes * 0.65)
    d->x += XRes * 0.1;
}

void move_disc_left(Disc *d) {
  if (d->x >= XRes * 0.15)
    d->x -= XRes * 0.1;
}

void move_disc_down(Disc *d) {
  d->y = YRes * 0.85;
}

Board *create_board() {
  //allocate space for the "disc"
  Board *board = (Board *) malloc(sizeof(Board));
  board->map = xpm_load(game_board, XPM_8_8_8, &(board->img));
  board->x = (XRes * 0.15)-5;
  board->y = 85;
  return board;
}

int draw_board(Board *board) {
  uint8_t *map = board->map;
  uint32_t color;
  unsigned bpp = bits_per_pixel / 8;

  for (int y = 0; y < board->img.height; y++) {
    for (int x = 0; x < board->img.width; x++) {
      color = 0;
      for (unsigned b = 0; b < bpp; b++) {
        color += *(map + b) << b * 8;
      }
      if (color != xpm_transparency_color(XPM_8_8_8)) {
        pixel_color(board->x + x, board->y + y, color);
        map += bpp;
      }
    }
  }
  return 0;
}

int delete_board(Board *board) {
  free(board);
  return 0;
}
