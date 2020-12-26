#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include "game.h"
#include <stdbool.h>
#include <stdint.h>

extern unsigned int counter;

int init_graphics_mode() {
  uint16_t mode = 0x115;
  vg_init(mode);
  if (vbe_int_10(mode) != 0) {
    return 1;
  };
  return 0;
}

extern uint8_t packet_byte;

Game *initiate_game() {
  Game *game = (Game *) malloc(sizeof(Game));
  game->done = false;
  game->display = true;
  game->kbd_scancode = 0;
  game->yellow = create_disc(yellow_disc);
  game->red = create_disc(red_disc);
  game->board = create_board();
  game->yellow_turn = true;
  game->red_turn = false;
  game->mouse = new_mouse();
  draw_board(game->board);
  return game;
}

int update_game(Game *game) {
  uint8_t kbd_bit_num = 0, mouse_bit_num = 0, timer_bit_num = 0;
  int ipc_status;
  message msg;
  int r;
  int pCounter = 0;
  timer_subscribe_int(&timer_bit_num);
  uint32_t timer_irq_set = BIT(timer_bit_num);

  kbc_subscribe_int(&kbd_bit_num);
  uint32_t kbd_irq_set = BIT(kbd_bit_num);

  mouse_subscribe_int(&mouse_bit_num);
  uint32_t mouse_irq_set = BIT(mouse_bit_num);

  mouse_enable_data();

  while (!game->done) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                 /* hardware interrupt notification */
          if (msg.m_notify.interrupts & kbd_irq_set) { /* subscribed interrupt */
            kbc_ih();
            if (kbd_done) {
              game->kbd_scancode = scancode;
              if (game->kbd_scancode != 0) {
                if (game->kbd_scancode == 0x1C) { // enter pressed
                  change_turn(game);
                  vg_draw_rectangle(0, 0, XRes, 80, 0);
                  //draw_board(game->board);
                }
                if (game->kbd_scancode == 0x4B) { // left
                  vg_draw_rectangle(0, 0, XRes, 80, 0);
                  //draw_board(game->board);

                  check_turn_left(game);
                  break;
                }
                if (game->kbd_scancode == 0x4D) { // right
                  vg_draw_rectangle(0, 0, XRes, 80, 0);
                  //draw_board(game->board);
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
          if (msg.m_notify.interrupts & mouse_irq_set) {
            mouse_ih();
            game->mouse->pack[pCounter] = packet_byte;

            if ((game->mouse->pack[0] & BIT(3)) == 0) {
              pCounter = 0;
            }
            else {
              if (pCounter == 2) {
                display_game(game);
                pCounter = 0;
              }
              else
                pCounter++;
            }
          }
          if (msg.m_notify.interrupts & timer_irq_set) {
            timer_int_handler();
            if (counter % (sys_hz()/10) == 0) {
              // draw
               double_buffer_update();
            }
          }
        default:
          break;
      }
    }
  }

  kbc_unsubscribe_int();
  mouse_unsubscribe_int();
  timer_unsubscribe_int();
  mouse_disable_data_reporting();

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
  if (game->yellow_turn) {
    move_disc_down(game->yellow);
    draw_disc(game->yellow);
    game->yellow->y = 5;
  }
  else {
    move_disc_down(game->red);
    draw_disc(game->red);
    game->red->y = 5;
  }
  game->red_turn = !game->red_turn;
  game->yellow_turn = !game->yellow_turn;
}

void display_game(Game *game) {
  check_turn_draw(game);
  draw_mouse(game->mouse);
}

void exit_game(Game *game) {
  kbc_unsubscribe_int();
  free(game);
}
