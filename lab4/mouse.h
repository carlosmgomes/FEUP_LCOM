#ifndef __MOUSE_H
#define __MOUSE_H
#include <stdbool.h>
#include <stdint.h>


typedef enum{INIT, DRAWUP, HALF, DRAWDOWN} States;

uint8_t packet_byte;
int (mouse_subscribe_int) (uint8_t *bit_no);
int (mouse_unsubscribe_int)();
void (mouse_ih)();
void(get_mouse_packet)(struct packet *pp, uint8_t pack[3]);
int mouse_enable_data();
int mouse_disable_data_reporting();
int minix_default_cmd_byte();
bool draw_handler (uint8_t xlen, uint8_t tolerance, struct packet * pack);



#endif /* __MOUSE_H */
