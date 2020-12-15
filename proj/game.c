#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include "game.h"
#include <stdbool.h>
#include <stdint.h>

int init_graphics_mode() {
  uint16_t mode = 0x115;
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
  game->KBD_SET_IRQ = kbc_subscribe_int(&bit_num);
  game->yellow = create_disc(yellow_disc);
  game->red = create_disc(red_disc);
  game->board = create_board();
  game->yellow_turn = true;
  game->red_turn = false;
  draw_board(game->board);
  return game;
}

int update_game(Game *game) {
  uint8_t bit_num = 0;
  int ipc_status;
  message msg;
  int r;

  if (kbc_subscribe_int(&bit_num))
    return 1;
  uint32_t irq_set = BIT(bit_num);

  while (!game->done) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                             /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
            kbc_ih();
            if (kbd_done) {
              game->kbd_scancode = scancode;
              if (game->kbd_scancode != 0) {
                if (game->kbd_scancode == 0x1C) {
                  change_turn(game);
                  vg_draw_rectangle(0, 0, XRes, YRes, 0);
                  draw_board(game->board);
                }
                if (game->kbd_scancode == 0x4B) { // left
                  vg_draw_rectangle(0, 0, XRes, YRes, 0);
                  draw_board(game->board);

                  check_turn_left(game);
                  break;
                }
                if (game->kbd_scancode == 0x4D) { // right
                  vg_draw_rectangle(0, 0, XRes, YRes, 0);
                  draw_board(game->board);
                  check_turn_right(game);
                  break;
                }
                if (game->kbd_scancode == KBD_ESC) {
                  game->done = true;
                  printf("ESC detected");
                  break;
                }
                display_game(game);
              }
            }
          }
        default:
          break;
      }
    }
  }

  if (kbc_unsubscribe_int())
    return 1;

  return 0;
}

void check_turn_draw(Game *game) {
  if (game->red_turn) {
    draw_disc(game->red);
  }
  if (game->yellow_turn) {
    draw_disc(game->yellow);
  }
}

void check_turn_right(Game *game) {
  if (game->red_turn) {
    move_disc_right(game->red);
  }
  if (game->yellow_turn) {
    move_disc_right(game->yellow);
  }
}

void check_turn_left(Game *game) {
  if (game->red_turn) {
    move_disc_left(game->red);
  }
  if (game->yellow_turn) {
    move_disc_left(game->yellow);
  }
}

void change_turn(Game *game) {
  game->red_turn = !game->red_turn;
  game->yellow_turn = !game->yellow_turn;
}

void display_game(Game *game) {
  check_turn_draw(game);
}

void exit_game(Game *game) {
  kbc_unsubscribe_int();
  free(game);
}
