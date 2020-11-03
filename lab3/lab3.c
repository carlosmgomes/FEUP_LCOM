#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include "i8042.h"
#include "utils.h"
#include <minix/sysutil.h>
#include "keyboard.h"
#include "timer.h"

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
extern uint8_t scancode[2];

int(kbd_test_poll)() {
  uint8_t scancode_bytes [2];

while(scancode[0]!=KBD_ESC_BREAKCODE){      //ends when it reads the breakcode (ESC key = 0x81)
kbc_ih();
if(scancode[0]==KBD_TWOBYTES_SCANCODE){   // if scancode has 2bytes
  scancode_bytes[1] = scancode[1];
  kbd_print_scancode(make,size,scancode_bytes);
  }
else{                                     // if scancode has 1byte
  scancode_bytes [0] = scancode[0];
  kbd_print_scancode(make,size,scancode_bytes);
  }

}
kbd_print_no_sysinb(siCounter);            // prints number of sys_inb calls
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

extern unsigned int counter


int(kbd_test_timed_scan)(uint8_t n) {
	uint8_t bitn;
  uint8_t bit_num;
  uint8_t timePassed = 0;

	if (kbc_subscribe_int(&bit_num)) return 1;
  uint32_t irq_set = BIT(bit_num);

  if (timer_subscribe_int(&bitn)) return 1;
  uint32_t timer_irq_set = (uint32_t)(BIT(bitn));

	message msg;
  int r, ipc_status;

 while ((scancode[0] != 0x81) && timePassed < n) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
			if (msg.m_notify.interrupts & timer_irq_set) { /* subscribed interrupt */
				timer_int_handler();
				if ((counter % 60) == 0) {
					timePassed++; //increases variable representing numbers of seconds passed
        }																
			}
			if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
				kbc_ih();	
				counter = 0;		
				timePassed = 0;
				if(done) kbd_print_scancode(make, size, scancode);
			}
			break;
		default:
			break; /* no other notifications expected: do nothing */
		}
	}
	}


	timer_unsubscribe_int();
    kbc_unsubscribe_int();

	return 0;
}
