#include "graphics_mode.h"
#include "i8042.h"
#include "keyboard.h"
#include "timer.h"
#include "mouse.h"
#include "sprite.h"
#include "xpm.h"

typedef struct{

int KBD_SET_IRQ;
bool done,display,red_turn,yellow_turn;
uint8_t kbd_scancode;
Board *board;
Disc *yellow;
Disc *red;
}Game;

Game* initiate_game();
int update_game(Game* game);
void display_game(Game* game);
void exit_game(Game* game);
int init_graphics_mode();
void change_turn(Game* game);
void check_turn_draw(Game* game);
void check_turn_right(Game *game);
void check_turn_left(Game *game);
