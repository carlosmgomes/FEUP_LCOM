#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#include <stdbool.h>
#include <stdint.h>

/** @defgroup keyboard keyboard
 * @{
 *
 * Functions for using the keyboard
 */

uint32_t siCounter;
uint8_t scancode_byte[2];
uint8_t scancode;

/**
 * @brief Subscribes keyboard interrupts
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int (kbc_subscribe_int) (uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_unsubscribe_int)();

/**
 * @brief Gets the scan code input from the keyboard.
 * @return Returns the scan code input from the keyboard
 */
int (get_kdb_scancode)(uint8_t *code);

uint8_t size;
bool kbd_done;
bool make;

/**
 * @brief handles the interruptions
 * 
 */
void (kbc_ih)();


#endif /* __KEYBOARD_H */
