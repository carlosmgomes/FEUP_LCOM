#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#define KBD_IRC 1


//  STATUS

#define KBC_PARITY BIT(7)
#define KBC_TIMEOUT BIT(6)
#define KBC_AUX BIT(5)
#define KBC_INH BIT(4)
#define KBC_A2 BIT(3)
#define KBC_SYS BIT(2)
#define KBC_IBF BIT(1)
#define KBC_OBF BIT(0)

// PORTS ADDRESSES

#define KBC_SR 0x64 // status register
#define KBC_CR 0x64 // command register 
#define KBC_IB 0x60 // input buffer
#define KBC_OB 0x60 // output buffer

// KBC COMMANDS

#define KBC_RCM 0x20 // read command byte
#define KBC_WCB 0x60 // write command byte
#define KBC_ST 0xAA // self test
#define KBC_CKI 0xAB // check keyboard interface
#define KBC_DKI 0xAD // disable keyboard interface
#define KBC_EKI 0xAE // enable keyboard interface

// KBC COMMAND BYTE

#define KBC_DIS2 BIT(5) // disable mouse
#define KBC_DIS BIT(4) // disable keyboard
#define KBC_INT2 BIT(1) // enable interrupt on obf from mouse
#define KBC_INT BIT(0) // enable interrupt on obf from keyboard

// OTHER

#define KBD_DELAY 20000 // keyboard delay
#define KBD_ESC_BREAKCODE 0x81 // breakcode of the ESC key
#define KBD_TWOBYTES_SCANCODE 0xE0 // first byte of two byte scancode


//MOUSE

#define MOUSE_IRQ 12

//  STATUS

#define LB BIT(0)
#define RB BIT(1)
#define MB BIT(2)
#define MSB_X_DELTA BIT(4)
#define MSB_Y_DELTA BIT(5)
#define X_OVFL BIT(6)
#define Y_OVFL BIT(7)

// MOUSE COMMANDS

#define DIS_MOUSE 0xA7 // disable mouse
#define ENABLE_MOUSE 0xA8 // enable mouse
#define CHECK_MOUSE 0xA9 // check mouse interface
#define WRT_BYTE_MOUSE 0xD4 // write byte to mouse
#define ACK 0xFA   // if everything OK
#define NACK 0xFE  // if invalid byte
#define ERROR 0xFC // second consecutive invalid byte
#define ENABLE 0xF4 // Enable (Data Reporting) In stream mode only
#define DISABLE 0xF5 // Disable (Data Reporting) In stream mode, should be sentbefore any other command
#define READ_DATA 0xEB // Send data packet request
#define SET_STREAM_MODE 0xEA // set Stream Mode Send data on events
#define SET_REMOTE_MODE 0xF0 //Set Remote mode Send data on request only





#endif /* _LCOM_I8042_H_ */
