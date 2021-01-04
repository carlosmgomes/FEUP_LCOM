#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#define KBD_IRC 1

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the keyboard
 */
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


/// Keys
typedef enum {
    KBD_NONE = 0x0000,
    KBD_ESC = 0x0001,
    KBD_1 = 0x0002,
    KBD_2 = 0x0003,
    KBD_3 = 0x0004,
    KBD_4 = 0x0005,
    KBD_5 = 0x0006,
    KBD_6 = 0x0007,
    KBD_7 = 0x0008,
    KBD_8 = 0x0009,
    KBD_9 = 0x000A,
    KBD_0 = 0x000B,
    KBD_APOSTROPHE = 0x000C,
    KBD_ANGLE_QUOTES = 0x000D,
    KBD_BKSP = 0x000E,
    KBD_TAB = 0x000F,
    KBD_Q = 0x0010,
    KBD_W = 0x0011,
    KBD_E = 0x0012,
    KBD_R = 0x0013,
    KBD_T = 0x0014,
    KBD_Y = 0x0015,
    KBD_U = 0x0016,
    KBD_I = 0x0017,
    KBD_O = 0x0018,
    KBD_P = 0x0019,
    KBD_PLUS = 0x001A,
    KBD_ACCENT = 0x001B,
    KBD_ENTER = 0x001C,
    KBD_L_CTRL = 0x001D,
    KBD_A = 0x001E,
    KBD_S = 0x001F,
    KBD_D = 0x0020,
    KBD_F = 0x0021,
    KBD_G = 0x0022,
    KBD_H = 0x0023,
    KBD_J = 0x0024,
    KBD_K = 0x0025,
    KBD_L = 0x0026,
    KBD_C_CEDILLA = 0x0027,
    KBD_ORDINAL = 0x0028,
    KBD_BACKSLASH = 0x0029,
    KBD_L_SHIFT = 0x002A,
    KBD_TILDE = 0x002B,
    KBD_Z = 0x002C,
    KBD_X = 0x002D,
    KBD_C = 0x002E,
    KBD_V = 0x002F,
    KBD_B = 0x0030,
    KBD_N = 0x0031,
    KBD_M = 0x0032,
    KBD_COMMA = 0x0033,
    KBD_DOT = 0x0034,
    KBD_MINUS = 0x0035,
    KBD_R_SHIFT = 0x0036,
    KBD_ALT = 0x0038,
    KBD_SPACE = 0x0039,
    KBD_CAPS = 0x003A,
    KBD_F1 = 0x003B,
    KBD_F2 = 0x003C,
    KBD_F3 = 0x003D,
    KBD_F4 = 0x003E,
    KBD_F5 = 0x003F,
    KBD_F6 = 0x0040,
    KBD_F7 = 0x0041,
    KBD_F8 = 0x0042,
    KBD_F9 = 0x0043,
    KBD_F10 = 0x0044,
    KBD_NUM = 0x0045,
    KBD_SCRLL = 0x0046,
    KBD_NUM_7 = 0x0047,
    KBD_NUM_8 = 0x0048,
    KBD_NUM_9 = 0x0049,
    KBD_NUM_MINUS = 0x004A,
    KBD_NUM_4 = 0x004B,
    KBD_NUM_5 = 0x004C,
    KBD_NUM_6 = 0x004D,
    KBD_NUM_PLUS = 0x004E,
    KBD_NUM_1 = 0x004F,
    KBD_NUM_2 = 0x0050,
    KBD_NUM_3 = 0x0051,
    KBD_NUM_0 = 0x0052,
    KBD_NUM_DEL = 0x0053,
    KBD_MINOR = 0x0056,
    KBD_F11 = 0x0057,
    KBD_F12 = 0x0058,
    KBD_NUM_ENTER = 0xE01C,
    KBD_R_CTRL = 0xE01D,
    KBD_NUM_SLASH = 0xE035,
    KBD_ALT_GR = 0xE038,
    KBD_HOME = 0xE047,
    KBD_ARR_UP = 0xE048,
    KBD_PGUP = 0xE049,
    KBD_ARR_LEFT = 0xE04B,
    KBD_ARR_RIGHT = 0xE04D,
    KBD_ARR_DOWN = 0xE050,
    KBD_PGDN = 0xE051,
    KBD_INS = 0xE052,
    KBD_DEL = 0xE053,
    KBD_WIN = 0xE05B,
    KBD_CNTX = 0xE05D,
    KBD_END = 0xE04F
} KEY;




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
