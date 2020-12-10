#include "graphics_mode.h"
#include "i8042.h"
#include "keyboard.h"
#include "timer.h"
#include "mouse.h"

typedef struct{

bool done,display;
uint8_t kbd_scancode;
}Game;

Game* initiate_game();
void update_game(Game* game);
void display_game(Game* game);
void exit_game(Game* game);
int init_graphics_mode();
