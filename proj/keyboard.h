#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#include <stdbool.h>
#include <stdint.h>


uint32_t siCounter;
uint8_t scancode_byte[2];
uint8_t scancode;


int (kbc_subscribe_int) (uint8_t *bit_no);

int (kbc_unsubscribe_int)();


int (get_kdb_scancode)(uint8_t *code);

uint8_t size;
bool kbd_done;
bool make;


void (kbc_ih)();


#endif /* __KEYBOARD_H */
