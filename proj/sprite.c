#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include "graphics_mode.h"
#include "sprite.h"
#include "xpm.h"
#include <stdbool.h>
#include <stdint.h>

Disc *create_disc() {
  //allocate space for the "object"
  Disc *d = (Disc *) malloc(sizeof(Disc));
  xpm_load(circle, XPM_8_8_8, &d->disc[0]);
  d->x = 125;
  d->y = 25;
  return d;
}

int draw_disc(Disc *d) {
  unsigned index = 0;
  for (int i = 0; i < d->disc[0].height; ++i){
    for (int j = 0;j<d->disc[0].width;++j){
      if(d->disc[0].bytes[index]!=xpm_transparency_color(XPM_8_8_8)){
        pixel_color(d->x+j,d->y+i,d->disc[0].bytes[index]);
      }
    }
  }
    return 0;
}



int delete_disc(Disc *d) {
  free(d);
  return 0;
}
