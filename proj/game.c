#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include "bitmap.h"
#include "game.h"
#include <stdbool.h>
#include <stdint.h>

int init_graphics_mode() {
  uint16_t mode = 0x11a;
  vg_init(mode);
  if (vbe_int_10(mode) != 0) {
    return 1;
  };
  return 0;
}

Game *initiate_game() {
  Game *game = (Game *) malloc(sizeof(Game));

  game->done = false;
  game->display = true;
  game->kbd_scancode = 0;
  uint8_t bit_num = 0;

  game->IRQ_KB = if (kbc_subscribe_int(&bit_num)){
    printf("kbc_subscribe_int error");
  }
  return game;
}

void update_game(Game *game) {
  int ipc_status;
  message msg;
  int r;

  if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
    printf("driver_receive failed with: %d", r);
  }
  if (is_ipc_notify(ipc_status)) { /* received notification */
    switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE:                             /* hardware interrupt notification */
        if (msg.m_notify.interrupts & game->IRQ_KB) { /* subscribed interrupt */
          kbc_ih();
          game->kbd_scancode= scancode;
          break;
        }
      default:
        break;
    }
  }
  if(game->kbd_scancode!=0){
    if(game->kbd_scancode==KBD_ESC){
      game->done=true;
      printf("ESC detected");
    }
  }
}

void display_game(Game *game){
  vg_draw_rectangle(0,0,XRes,YRes,0x222222);
}

void exit_game(Game* game){
  kbc_unsubscribe_int();
  free(game);
}
