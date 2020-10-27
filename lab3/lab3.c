#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include "i8042.h"
#include "utils.h"
#include <minix/sysutil.h>

#include <stdbool.h>
#include <stdint.h>


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}
extern uint32_t counter; // variable updated in utils.c
int(kbd_test_scan)() {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(kbd_test_poll)() {
  uint8_t size, scancode_bytes [2];

while(scancode!=KBD_ESC_BREAKCODE){      //ends when it reads the breakcode (ESC key = 0x81)
kbc_ih();
if(scancode==KBD_TWOBYTES_SCANCODE){   // if scancode has 2bytes
  scancode_bytes[1] = scancode>>4;
  kbd_print_scancode(1,2,scancode_bytes);
  }
else{                                     // if scancode has 1byte
  scancode_bytes [0] = scancode;
  kbd_print_scancode(1,1,scancode_bytes);
  }

}
#define LAB3
kbd_print_no_sysinb(counter);            // prints number of sys_inb calls
#ifdef LAB3
#endif
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
