#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#include <stdbool.h>
#include <stdint.h>


int kbd_hid;
uint32_t siCounter;
uint8_t scancode[2];


int (kbc_subscribe_int) (uint8_t *bit_no);

int (kbc_unsubscribe_int)();


int (get_kdb_scancode)(uint8_t *scancode);
void kdb_enable_interruptions();

uint8_t size;
bool done;
bool make;


void (kbc_ih)();


#endif /* __KEYBOARD_H */
