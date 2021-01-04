#pragma once

#include "graphics_mode.h"
#include "i8042.h"
#include "keyboard.h"
#include "timer.h"
#include "mouse.h"
#include "sprite.h"
#include "xpm.h"
#include "mouse.h"
#include "main_menu.h"

/** @defgroup game game
 * @{
 *
 * Functions relating to game state and game logic
 */

typedef enum{
  MENU_STATE, GAME_STATE, END_STATE, INSTRUCTIONS_STATE
} Game_State;

typedef struct{

bool done,display,red_turn,yellow_turn,yellow_win, red_win, tie;
uint8_t kbd_scancode;
Board *board;
Disc *yellow;
Disc *red;
Disc *yellow_board;
Disc *red_board;
Background *mainmenu;
Background *instructions;
Background *endgame_yellow;
Background *endgame_red;
Background *endgame_tie;
Mouse *mouse;
Game_State state;
uint32_t timer_irq_set, kbd_irq_set,mouse_irq_set;
}Game;

/**
 * @brief Initiates the game and subscribes interruptions
 * 
 * @return Game* 
 */
Game* initiate_game();



/**
 * @brief subscribes all the device interruptions
 * 
 * @param game game pointer to an object of game struct
 */
void subscribe_interruptions(Game* game);

/**
 * @brief handles all the device interruptions
 * 
 * @param game game pointer to an object of game struct
 * @return int 0 upon success, non-zero upon failure
 */
int update_game(Game* game);

/**
 * @brief draws the game state on the screen
 * 
 * @param game game pointer to an object of game struct
 */
void display_game(Game* game);

/**
 * @brief exits the game and unsubscribes interruptions
 * 
 * @param game game pointer to an object of game struct
 */
void exit_game(Game* game);

/**
 * @brief Initializes the video module in graphics mode
 * 
 * @return int 0 upon success, non-zero upon failure
 */
int init_graphics_mode();

/**
 * @brief registers the player move and changes player turn
 * 
 * @param game game pointer to an object of game struct
 */
void change_turn(Game* game);

/**
 * @brief draws the disc on top of the board
 * 
 * @param game game pointer to an object of game struct
 */
void check_turn_draw(Game* game);

/**
 * @brief moves the disc to the next column on the right, if possible
 * 
 * @param game game pointer to an object of game struct
 */
void check_turn_right(Game *game);

/**
 * @brief moves the disc to the next column on the left, if possible
 * 
 * @param game game pointer to an object of game struct
 */
void check_turn_left(Game *game);

/**
 * @brief initializes the game board, setting all the columns arrays to zero
 * 
 * @param board board pointer to an object of board struct
 */
void init_board(Board *board);

/**
 * @brief draws the discs on the board, according to the board array
 * 
 * @param game game pointer to an object of game struct
 */
void fill_board(Game *game);

/**
 * @brief draws the disc on the board based on the array coordinates
 * 
 * @param d disc pointer to an object of disc struct
 * @param x1 hole x coordinate
 * @param y1 hole y coordinate
 * @return int 0 upon success, non-zero upon failure
 */
int draw_disc_on_coords(Disc *d, int x1, int y1);

/**
 * @brief updates the column array with the new disc placed
 * 
 * @param game game pointer to an object of game struct
 */
void place_disc_array(Game* game);

/**
 * @brief checks if the turn is valid
 * 
 * @param game game pointer to an object of game struct
 * @return true 
 * @return false 
 */
bool check_turn(Game* game);

/**
 * @brief checks if the previous move results in a winning scenario on the vertical direction
 * 
 * @param game game pointer to an object of game struct
 * @param row row number that the previous disc was played on
 * @return true if it resulted in a victory
 * @return false if not
 */
bool check_win_vertical(Game* game, int row);

/**
 * @brief checks if the previous move results in a winning scenario on the horizontal direction
 * 
 * @param game game pointer to an object of game struct
 * @param row row number that the previous disc was played on
 * @return true if it resulted in a victory
 * @return false if not
 */
bool check_win_horizontal(Game* game, int row);

/**
 * @brief checks if the previous move results in a winning scenario in all directions
 * 
 * @param game game pointer to an object of game struct
 * @param row row number that the previous disc was played on
 * @return true if it resulted in a victory
 * @return false if not
 */
bool check_win(Game* game, int row);

/**
 * @brief checks if the previous move results in a winning scenario on the diagonal direction
 * 
 * @param game game pointer to an object of game struct
 * @return true if it resulted in a victory
 * @return false if not
 */
bool check_win_diagonal(Game* game);

/**
 * @brief handles the keyboard interruptions
 * 
 * @param game game pointer to an object of game struct
 */
void kbd_game_handler(Game* game);

/**
 * @brief handles the mouse interruptions
 * 
 * @param game game pointer to an object of game struct
 */
void mouse_game_handler(Game* game);

/**
 * @brief makes the disc follow the mouse cursor when its the yellow player turn
 * 
 * @param game game pointer to an object of game struct
 */
void mouse_follow_disc(Game * game);

/**
 * @brief checks who won the game
 * 
 * @param game game pointer to an object of game struct
 * @param color color of the player who made the last move
 */
void check_win_color(Game *game,int color);

/**
 * @brief verifys if the board is full with no winners, resulting in a tie
 * 
 * @param game game pointer to an object of game struct
 */
void verify_full_board(Game *game);


void piece_animation(Game *game, Disc *d);
