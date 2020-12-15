#include <lcom/lcf.h>
/**
 *  @author Joao Cardoso (jcard@fe.up.pt) ????
 *  Added by pfs@fe.up.pt
 */

#ifndef _SPRITE_H_
#define _SPRITE_H_
/** @defgroup sprite Sprite
 * @{
 *
 * Sprite related functions
 */

/** A Sprite is an "object" that contains all needed information to
 * create, animate, and destroy a pixmap.  The functions assume that
 * the background is BLACK and they take into account collision with
 * other graphical objects or the screen limits. 
 */
typedef struct {
  int x,y;             /**< current sprite position */
  xpm_image_t img; /**< the sprite pixmap (use read_xpm()) */
  uint8_t *map;           
} Disc;

/** Creates with random speeds (not zero) and position
 * (within the screen limits), a new sprite with pixmap "pic", in
 * memory whose address is "base";
 * Returns NULL on invalid pixmap.
 */
Disc *create_disc(const char *circle[]);

int delete_disc(Disc *d);

void move_disc_right(Disc *d);

void move_disc_down(Disc *d);

void move_disc_left(Disc *d);
/** @} end of sprite */

int draw_disc(Disc *d);


typedef struct {
  int x,y;             /**< current sprite position */
  xpm_image_t img; /**< the sprite pixmap (use read_xpm()) */
  uint8_t *map;           
} Board;

/** Creates with random speeds (not zero) and position
 * (within the screen limits), a new sprite with pixmap "pic", in
 * memory whose address is "base";
 * Returns NULL on invalid pixmap.
 */
Board *create_board();

int delete_board(Board *board);

/** @} end of sprite */

int draw_board(Board *board);

#endif
