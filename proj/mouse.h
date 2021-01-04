#ifndef __MOUSE_H
#define __MOUSE_H
#include <stdbool.h>
#include <stdint.h>

/** @defgroup mouse mouse
 * @{
 *
 * Functions for using the mouse
 */
typedef enum{INIT, DRAWUP, HALF, DRAWDOWN} States;
struct mouse_ev mouse_event;

typedef struct {
    int x, y;
    int w, h;
    int lb;
    uint8_t pack[3];
} Mouse;

/**
 * @brief itiniates a new mouse
 * 
 * @return Mouse* pointer to an object of mouse struct
 */
Mouse* new_mouse();

/**
 * @brief draws the mouse cursor on the screen
 * 
 * @param mouse mouse pointer to an object of mouse struct
 */
void draw_mouse(Mouse* mouse);

uint8_t packet_byte;

/**
 * @brief subscribes mouse interruptions
 * 
 */
int (mouse_subscribe_int) (uint8_t *bit_no);


/**
 * @brief unsubscribes mouse interruptions
 * 
 */
int (mouse_unsubscribe_int)();

/**
 * @brief handles mouse interruptions
 * 
 */
void (mouse_ih)();

/**
 * @brief reads the mouse packet from the output buffer
 * 
 */
void(get_mouse_packet)(struct packet *pp, uint8_t pack[3]);

/**
 * @brief enables mouse data reporting
 * 
 * @return int 0 upon success non zero upon failure
 */
int mouse_enable_data();

/**
 * @brief disables mouse data reporting
 * 
 * @return int 0 upon success non zero upon failure
 */
int mouse_disable_data_reporting();

/**
 * @brief minix default command byte function
 * 
 * @return int 0 upon success non zero upon failure
 */
int minix_default_cmd_byte();

/**
 * @brief handles mouse events
 * 
 * @param xlen xlen value
 * @param tolerance tolerance value
 * @param pack pack pointer to object of pack struct
 * @return true upon success
 * @return false upon failure
 */
bool draw_handler (uint8_t xlen, uint8_t tolerance, struct packet * pack);

/**
 * @brief handles mouse events
 * 
 * @param pack pack pointer to object of pack struct
 * @return struct mouse_ev* mouse event
 */
struct mouse_ev * mouse_events(struct packet * pack);



#endif /* __MOUSE_H */
