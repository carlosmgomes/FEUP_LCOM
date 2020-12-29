#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include "main_menu.h"
#include <stdbool.h>
#include <stdint.h>


const int play_x1 = 319;
const int play_x2 = 526;
const int play_y1 = 261;
const int play_y2 = 340;

const int exit_x1 = 332;
const int exit_x2 = 513;
const int exit_y1 = 393;
const int exit_y2 = 478;

bool play_choose(Mouse *mouse) {
  if ((mouse->x > play_x1) && (mouse->x < play_x2) && (mouse->y > play_y1) && (mouse->y < play_y2)) {
    struct mouse_ev *mouse_event = mouse_events(mouse->pp);
    switch (mouse_event->type) {
      case LB_PRESSED:
        return true;

      default:
        return false;
    }
  }
  return false;
}

bool exit_choose(Mouse *mouse) {
  if ((mouse->x > exit_x1) && (mouse->x < exit_x2) && (mouse->y > exit_y1) && (mouse->y < exit_y2)) {
    struct mouse_ev *mouse_event = mouse_events(mouse->pp);
    switch (mouse_event->type) {
      case LB_PRESSED:
        return true;

      default:
        return false;
    }
  }
  return false;
}
