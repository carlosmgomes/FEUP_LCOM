#include "graphics_mode.h"
#include "i8042.h"
#include "keyboard.h"
#include "timer.h"
#include "mouse.h"
#include "sprite.h"
#include "xpm.h"
#include "mouse.h"

typedef struct{

bool done,display,red_turn,yellow_turn;
uint8_t kbd_scancode;
Board *board;
Disc *yellow;
Disc *red;
Mouse *mouse;
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
void init_board(Board *board);
void fill_board(Game *game);
int draw_disc_on_coords(Disc *d, int x1, int y1);
void place_disc_array(Game* game);
bool check_turn(Game* game);
bool check_win_vertical(Game* game, int row);
bool check_win_horizontal(Game* game, int row);
bool check_win(Game* game, int row);
bool check_win_diagonal(Game* game);



