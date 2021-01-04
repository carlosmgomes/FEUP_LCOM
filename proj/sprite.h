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
  int column;        
} Disc;

/** Creates with random speeds (not zero) and position
 * (within the screen limits), a new sprite with pixmap "pic", in
 * memory whose address is "base";
 * Returns NULL on invalid pixmap.
 */
Disc *create_disc(const char *circle[]);

/**
 * @brief deletes the disc object from memory
 * 
 * @param d disc to be deleted
 * @return int 0 upon success, non zero upon failure
 */
int delete_disc(Disc *d);

/**
 * @brief moves the disc to the right
 * 
 * @param d disc pointer to disc object struct
 */
void move_disc_right(Disc *d);

/**
 * @brief moves the disc down
 * 
 * @param d disc pointer to disc object struct
 */
void move_disc_down(Disc *d);

/**
 * @brief moves the disc to the left
 * 
 * @param d disc pointer to disc object struct
 */
void move_disc_left(Disc *d);


/**
 * @brief draws the disc on the screen
 * 
 * @param d disc pointer to disc object struct
 * @return int 0 upon success, non zero upon failure
 */
int draw_disc(Disc *d);


typedef struct { // 0 = empty, 1 = yellow, 2 = red;
  int x,y;             /**< current sprite position */
  xpm_image_t img; /**< the sprite pixmap (use read_xpm()) */
  uint8_t *map;
  uint8_t Column1[6], Column2[6], Column3[6], Column4[6], Column5[6], Column6[6], Column7[6];            
} Board;

/** Creates with random speeds (not zero) and position
 * (within the screen limits), a new sprite with pixmap "pic", in
 * memory whose address is "base";
 * Returns NULL on invalid pixmap.
 */
Board *create_board();

/**
 * @brief deletes the board object from memory
 * 
 * @param board board to be deleted
 * @return int 0 upon success, non zero upon failure
 */
int delete_board(Board *board);

/** @} end of sprite */

/**
 * @brief draws the board on the screen
 * 
 * @param board board pointer to board object struct
 * @return int 0 upon success, non zero upon failure
 */
int draw_board(Board *board);


typedef struct {
  int x,y;             /**< current sprite position */
  xpm_image_t img; /**< the sprite pixmap (use read_xpm()) */
  uint8_t *map;   
} Background;

/** Creates with random speeds (not zero) and position
 * (within the screen limits), a new sprite with pixmap "pic", in
 * memory whose address is "base";
 * Returns NULL on invalid pixmap.
 */
Background *create_background(const char *image[]);

/**
 * @brief deletes the background object from memory
 * 
 * @param bg background to be deleted
 * @return int 0 upon success, non zero upon failure
 */
int delete_background(Background *bg);


/**
 * @brief draws the background on the screen
 * 
 * @param bg background pointer to background object struct
 * @return int 0 upon success, non zero upon failure
 */
int draw_background(Background *bg);

#endif
