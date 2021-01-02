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
  game->mainmenu = create_background(main_menu_bg);
  game->endgame_red = create_background(endgame_red);
  game->endgame_yellow = create_background(endgame_yellow);
  game->board = create_board();
  game->yellow_turn = true;
  game->red_turn = false;
  game->mouse = new_mouse();
  game->state = MENU_STATE;
  game->yellow_win = false;
  game->red_win = false;
  init_board(game->board);
  display_game(game);
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
              kbd_game_handler(game);
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
                mouse_game_handler(game);
                pCounter = 0;
              }
              else
                pCounter++;
            }
          }
          if (msg.m_notify.interrupts & timer_irq_set) {
            timer_int_handler();
            /*if (counter % (sys_hz() / 10) == 0) {
              double_buffer_update();
            }*/
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
  place_disc_array(game);
  display_game(game);
  //piece_animation(game);
  game->red_turn = !game->red_turn;
  game->yellow_turn = !game->yellow_turn;
}

void display_game(Game *game) {
  switch (game->state) {
    case GAME_STATE:
      check_turn_draw(game);
      draw_mouse(game->mouse);
      fill_board(game);
      if (game->yellow_turn) {
        draw_disc(game->yellow);
      }
      else {
        draw_disc(game->red);
      }
      if (game->yellow_win || game->red_win) {
        game->state = END_STATE;
        display_game(game);
      }
      break;
    case MENU_STATE:
      vg_draw_rectangle(0, 0, XRes, YRes, 0);
      game->mainmenu = create_background(main_menu_bg);
      draw_background(game->mainmenu);
      break;
    case END_STATE:
      sleep(2);
      vg_draw_rectangle(0, 0, XRes, YRes, 0);
      if (game->yellow_win) {
        game->mainmenu = create_background(endgame_yellow);
        draw_background(game->endgame_yellow);
      }
      if (game->red_win) {
        game->mainmenu = create_background(endgame_red);
        draw_background(game->endgame_red);
      }
      sleep(7);
      game->state = MENU_STATE;
      game->yellow_turn = false;
      game->red_turn = true;
      game->yellow_win = false;
      game->red_win = false;
      init_board(game->board);
      break;
  }
}

void exit_game(Game *game) {
  kbc_unsubscribe_int();
  free(game);
}

void init_board(Board *board) {
  for (int i = 0; i < 6; i++) board->Column1[i] = 0;
  for (int i = 0; i < 6; i++) board->Column2[i] = 0;
  for (int i = 0; i < 6; i++) board->Column3[i] = 0;
  for (int i = 0; i < 6; i++) board->Column4[i] = 0;
  for (int i = 0; i < 6; i++) board->Column5[i] = 0;
  for (int i = 0; i < 6; i++) board->Column6[i] = 0;
  for (int i = 0; i < 6; i++) board->Column7[i] = 0;
  return;
}

void fill_board(Game *game) {
  for (int i = 0; i < 6; i++) {
    if (game->board->Column1[i] == 1)
      draw_disc_on_coords(game->yellow, 122, (512 - 83 * i));
    else if (game->board->Column1[i] == 2)
      draw_disc_on_coords(game->red, 122, (512 - 83 * i));
    if (game->board->Column2[i] == 1)
      draw_disc_on_coords(game->yellow, 205, (512 - 83 * i));
    else if (game->board->Column2[i] == 2)
      draw_disc_on_coords(game->red, 205, (512 - 83 * i));
    if (game->board->Column3[i] == 1)
      draw_disc_on_coords(game->yellow, 288, (512 - 83 * i));
    else if (game->board->Column3[i] == 2)
      draw_disc_on_coords(game->red, 288, (512 - 83 * i));
    if (game->board->Column4[i] == 1)
      draw_disc_on_coords(game->yellow, 371, (512 - 83 * i));
    else if (game->board->Column4[i] == 2)
      draw_disc_on_coords(game->red, 371, (512 - 83 * i));
    if (game->board->Column5[i] == 1)
      draw_disc_on_coords(game->yellow, 454, (512 - 83 * i));
    else if (game->board->Column5[i] == 2)
      draw_disc_on_coords(game->red, 454, (512 - 83 * i));
    if (game->board->Column6[i] == 1)
      draw_disc_on_coords(game->yellow, 537, (512 - 83 * i));
    else if (game->board->Column6[i] == 2)
      draw_disc_on_coords(game->red, 537, (512 - 83 * i));
    if (game->board->Column7[i] == 1)
      draw_disc_on_coords(game->yellow, 620, (512 - 83 * i));
    else if (game->board->Column7[i] == 2)
      draw_disc_on_coords(game->red, 620, (512 - 83 * i));
  }
}

int draw_disc_on_coords(Disc *d, int x1, int y1) {
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
        pixel_color(x1 + x, y1 + y, color);
        map += bpp;
      }
    }
  }
  return 0;
}

void place_disc_array(Game *game) {
  int color = 0;
  int column = 0;
  int row = 0;
  if (game->yellow_turn) {
    color = 1;
    column = game->yellow->column;
  }
  else {
    color = 2;
    column = game->red->column;
  }

  if (column == 1) {
    for (int i = 0; i < 6; i++) {
      if (game->board->Column1[i] == 0) {
        game->board->Column1[i] = color;
        row = i;
        break;
      }
    }
  }
  if (column == 2) {
    for (int i = 0; i < 6; i++) {
      if (game->board->Column2[i] == 0) {
        game->board->Column2[i] = color;
        row = i;
        break;
      }
    }
  }
  if (column == 3) {
    for (int i = 0; i < 6; i++) {
      if (game->board->Column3[i] == 0) {
        game->board->Column3[i] = color;
        row = i;
        break;
      }
    }
  }
  if (column == 4) {
    for (int i = 0; i < 6; i++) {
      if (game->board->Column4[i] == 0) {
        game->board->Column4[i] = color;
        row = i;
        break;
      }
    }
  }
  if (column == 5) {
    for (int i = 0; i < 6; i++) {
      if (game->board->Column5[i] == 0) {
        game->board->Column5[i] = color;
        row = i;
        break;
      }
    }
  }
  if (column == 6) {
    for (int i = 0; i < 6; i++) {
      if (game->board->Column6[i] == 0) {
        game->board->Column6[i] = color;
        row = i;
        break;
      }
    }
  }
  if (column == 7) {
    for (int i = 0; i < 6; i++) {
      if (game->board->Column7[i] == 0) {
        game->board->Column7[i] = color;
        row = i;
        break;
      }
    }
  }
  check_win(game, row);
}

bool check_turn(Game *game) {
  int column = 1;
  if (game->yellow_turn)
    column = game->yellow->column;
  else
    column = game->red->column;
  if (column == 1 && game->board->Column1[5] != 0)
    return false;
  if (column == 2 && game->board->Column2[5] != 0)
    return false;
  if (column == 3 && game->board->Column3[5] != 0)
    return false;
  if (column == 4 && game->board->Column4[5] != 0)
    return false;
  if (column == 5 && game->board->Column5[5] != 0)
    return false;
  if (column == 6 && game->board->Column6[5] != 0)
    return false;
  if (column == 7 && game->board->Column7[5] != 0)
    return false;
  return true;
}

bool check_win_vertical(Game *game, int row) {
  int color = 0;
  int column = 0;
  if (game->yellow_turn) {
    color = 1;
    column = game->yellow->column;
  }
  else {
    color = 2;
    column = game->red->column;
  }

  if (column == 1 && row > 2) {
    if (game->board->Column1[row] == color && game->board->Column1[row - 1] == color && game->board->Column1[row - 2] == color && game->board->Column1[row - 3] == color) {
      check_win_color(game, color);
      return true;
    }
  }
  if (column == 2 && row > 2) {
    if (game->board->Column2[row] == color && game->board->Column2[row - 1] == color && game->board->Column2[row - 2] == color && game->board->Column2[row - 3] == color) {
      check_win_color(game, color);
      return true;
    }
  }
  if (column == 3 && row > 2) {
    if (game->board->Column3[row] == color && game->board->Column3[row - 1] == color && game->board->Column3[row - 2] == color && game->board->Column3[row - 3] == color) {
      check_win_color(game, color);
      return true;
    }
  }
  if (column == 4 && row > 2) {
    if (game->board->Column4[row] == color && game->board->Column4[row - 1] == color && game->board->Column4[row - 2] == color && game->board->Column4[row - 3] == color) {
      check_win_color(game, color);
      return true;
    }
  }
  if (column == 5 && row > 2) {
    if (game->board->Column5[row] == color && game->board->Column5[row - 1] == color && game->board->Column5[row - 2] == color && game->board->Column5[row - 3] == color) {
      check_win_color(game, color);
      return true;
    }
  }
  if (column == 6 && row > 2) {
    if (game->board->Column6[row] == color && game->board->Column6[row - 1] == color && game->board->Column6[row - 2] == color && game->board->Column6[row - 3] == color) {
      check_win_color(game, color);
      return true;
    }
  }
  if (column == 7 && row > 2) {
    if (game->board->Column7[row] == color && game->board->Column7[row - 1] == color && game->board->Column7[row - 2] == color && game->board->Column7[row - 3] == color) {
      check_win_color(game, color);
      return true;
    }
  }
  return false;
}

bool check_win_horizontal(Game *game, int row) {
  int color = 0;
  int column = 0;
  if (game->yellow_turn) {
    color = 1;
    column = game->yellow->column;
  }
  else {
    color = 2;
    column = game->red->column;
  }
  if (column == 1 || column == 2 || column == 3 || column == 4) {
    if (game->board->Column1[row] == color && game->board->Column2[row] == color && game->board->Column3[row] == color && game->board->Column4[row] == color) {
      check_win_color(game, color);
      return true;
    }
  }
  if (column == 2 || column == 3 || column == 4 || column == 5) {
    if (game->board->Column2[row] == color && game->board->Column3[row] == color && game->board->Column4[row] == color && game->board->Column5[row] == color) {
      check_win_color(game, color);
      return true;
    }
  }
  if (column == 3 || column == 4 || column == 5 || column == 6) {
    if (game->board->Column3[row] == color && game->board->Column4[row] == color && game->board->Column5[row] == color && game->board->Column6[row] == color) {
      check_win_color(game, color);
      return true;
    }
  }
  if (column == 4 || column == 5 || column == 6 || column == 7) {
    if (game->board->Column4[row] == color && game->board->Column5[row] == color && game->board->Column6[row] == color && game->board->Column7[row] == color) {
      check_win_color(game, color);
      return true;
    }
  }
  return false;
}

bool check_win(Game *game, int row) {
  if (check_win_vertical(game, row))
    return true;
  else if (check_win_horizontal(game, row))
    return true;
  else if (check_win_diagonal(game))
    return true;
  return false;
}

bool check_win_diagonal(Game *game) {
  int color = 0;
  if (game->yellow_turn)
    color = 1;
  else
    color = 2;

  if (game->board->Column1[0] == color && game->board->Column2[1] == color && game->board->Column3[2] == color && game->board->Column4[3] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column2[0] == color && game->board->Column3[1] == color && game->board->Column4[2] == color && game->board->Column5[3] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column3[0] == color && game->board->Column4[1] == color && game->board->Column5[2] == color && game->board->Column6[3] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column4[0] == color && game->board->Column5[1] == color && game->board->Column6[2] == color && game->board->Column7[3] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column1[1] == color && game->board->Column2[2] == color && game->board->Column3[3] == color && game->board->Column4[4] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column2[1] == color && game->board->Column3[2] == color && game->board->Column4[3] == color && game->board->Column5[4] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column3[1] == color && game->board->Column4[2] == color && game->board->Column5[3] == color && game->board->Column6[4] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column4[1] == color && game->board->Column5[2] == color && game->board->Column6[3] == color && game->board->Column7[4] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column1[2] == color && game->board->Column2[3] == color && game->board->Column3[4] == color && game->board->Column4[5] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column2[2] == color && game->board->Column3[3] == color && game->board->Column4[4] == color && game->board->Column5[5] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column3[2] == color && game->board->Column4[3] == color && game->board->Column5[4] == color && game->board->Column6[5] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column4[2] == color && game->board->Column5[3] == color && game->board->Column6[4] == color && game->board->Column7[5] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column1[2] == color && game->board->Column2[3] == color && game->board->Column3[4] == color && game->board->Column4[5] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column1[5] == color && game->board->Column2[4] == color && game->board->Column3[3] == color && game->board->Column4[2] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column2[5] == color && game->board->Column3[4] == color && game->board->Column4[3] == color && game->board->Column5[2] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column3[5] == color && game->board->Column4[4] == color && game->board->Column5[3] == color && game->board->Column6[2] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column4[5] == color && game->board->Column5[4] == color && game->board->Column6[3] == color && game->board->Column7[2] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column1[4] == color && game->board->Column2[3] == color && game->board->Column3[2] == color && game->board->Column4[1] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column2[4] == color && game->board->Column3[3] == color && game->board->Column4[2] == color && game->board->Column5[1] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column3[4] == color && game->board->Column4[3] == color && game->board->Column5[2] == color && game->board->Column6[1] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column4[4] == color && game->board->Column5[3] == color && game->board->Column6[2] == color && game->board->Column7[1] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column1[3] == color && game->board->Column2[2] == color && game->board->Column3[1] == color && game->board->Column4[0] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column2[3] == color && game->board->Column3[2] == color && game->board->Column4[1] == color && game->board->Column5[0] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column3[3] == color && game->board->Column4[2] == color && game->board->Column5[1] == color && game->board->Column6[0] == color) {
    check_win_color(game, color);
    return true;
  }
  else if (game->board->Column4[3] == color && game->board->Column5[2] == color && game->board->Column6[1] == color && game->board->Column7[0] == color) {
    check_win_color(game, color);
    return true;
  }
  return false;
}

void kbd_game_handler(Game *game) {
  game->kbd_scancode = scancode;
  switch (game->state) {
    case GAME_STATE:
      if (game->kbd_scancode != 0) {
        if (game->red_turn) {
          if (game->kbd_scancode == 0x1C) { // enter pressed
            if (check_turn(game))
              change_turn(game);
            vg_draw_rectangle(0, 0, XRes, 80, 0);
          }
          if (game->kbd_scancode == 0x4B) { // left
            vg_draw_rectangle(0, 0, XRes, 80, 0);
            check_turn_left(game);
            break;
          }
          if (game->kbd_scancode == 0x4D) { // right
            vg_draw_rectangle(0, 0, XRes, 80, 0);
            check_turn_right(game);
            break;
          }
        }
        if (game->kbd_scancode == KBD_ESC) {
          game->state = MENU_STATE;
          display_game(game);
          break;
        }
        display_game(game);
        break;
      }
    case MENU_STATE:
      if (game->kbd_scancode != 0) {
        if (game->kbd_scancode == 0x1C) {
          game->state = GAME_STATE;
          vg_draw_rectangle(0, 0, XRes, YRes, 0);
          draw_board(game->board);
          init_board(game->board);
        }
        if (game->kbd_scancode == KBD_ESC) {
          game->done = true;
          printf("ESC detected");
          break;
        }
      }
    default:
      break;
  }
}

void mouse_game_handler(Game *game) {
  switch (game->state) {
    case GAME_STATE:
      if (game->yellow_turn) {
        if (game->mouse->pack[0] & BIT(0)) {
          if (check_turn(game))
            change_turn(game);
          vg_draw_rectangle(0, 0, XRes, 80, 0);
        }
        mouse_follow_disc(game);
        vg_draw_rectangle(0, 0, XRes, 80, 0);
        display_game(game);
        break;
      }
    case MENU_STATE:
      if (play_choose(game->mouse)) {
        game->state = GAME_STATE;
        vg_draw_rectangle(0, 0, XRes, YRes, 0);
        draw_board(game->board);
      }
      else if (exit_choose(game->mouse)) {
        exit_game(game);
      }

    default:
      break;
  }
}

// 122, 205, 288, 371, 454, 537, 620
void mouse_follow_disc(Game *game) {
  if (game->mouse->x <= 163) {
    game->yellow->column = 1;
    game->yellow->x = 122;
  }
  else if (game->mouse->x <= 247) {
    game->yellow->column = 2;
    game->yellow->x = 205;
  }
  else if (game->mouse->x <= 330) {
    game->yellow->column = 3;
    game->yellow->x = 288;
  }
  else if (game->mouse->x <= 413) {
    game->yellow->column = 4;
    game->yellow->x = 371;
  }
  else if (game->mouse->x <= 496) {
    game->yellow->column = 5;
    game->yellow->x = 454;
  }
  else if (game->mouse->x <= 579) {
    game->yellow->column = 6;
    game->yellow->x = 537;
  }
  else if (game->mouse->x >= 579) {
    game->yellow->column = 7;
    game->yellow->x = 620;
  }
  return;
}

void check_win_color(Game *game, int color) {
  if (color == 1) {
    game->yellow_win = true;
  }
  else if (color == 2) {
    game->red_win = true;
  }
  else {
    return;
  }
}
