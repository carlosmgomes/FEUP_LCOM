#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#include <stdbool.h>
#include <stdint.h>


uint8_t scancode;

int (kbc_subscribe_int) (uint8_t *bit_no);

int (kbc_unsubscribe_int)();

void (kbc_ih)();

int (get_kdb_scancode)(uint8_t *scancode);

void kdb_enable_interruptions();





#endif /* __KEYBOARD_H */

