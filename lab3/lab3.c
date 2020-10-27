#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include "i8042.h"
#include "utils.h"
#include <minix/sysutil.h>
#include <keyboard.c>

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
extern uint8_t scancode[2];

int(kbd_test_poll)() {
  uint8_t scancode_bytes [2];

while(scancode[0]!=KBD_ESC_BREAKCODE){      //ends when it reads the breakcode (ESC key = 0x81)
kbc_ih();
if(scancode[0]==KBD_TWOBYTES_SCANCODE){   // if scancode has 2bytes
  scancode_bytes[1] = scancode[1];
  kbd_print_scancode(1,2,scancode_bytes); // 1(true),2(size)
  }
else{                                     // if scancode has 1byte
  scancode_bytes [0] = scancode[0];
  kbd_print_scancode(1,1,scancode_bytes);  //1(true),1(size)
  }

}
#define LAB3
kbd_print_no_sysinb(counter);            // prints number of sys_inb calls
#ifdef LAB3
#endif
return 0;
}

int(kbd_test_scan)() {

	uint8_t bit_num = 0;
	int ipc_status;
	message msg;
	int r;
	
	if (kbc_subscribe_int(&bit_num)) return 1;
	uint32_t irq_set = BIT(bit_num);
	

	while (scancode[0] != 0x81)
	{
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
					kbc_ih();
          if (done) {
            kbd_print_scancode(make, size, scancode);
				}
				}
			default:
				break;
			}
		}
	}
                                                                                                    
	if(kbc_unsubscribe_int()) return 1;

	kbd_print_no_sysinb(siCounter);  
	return 0;

}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
