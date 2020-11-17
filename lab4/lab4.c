// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "i8042.h"
#include "mouse.h"
#include "timer.h"
#include "utils.h"


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
	uint8_t bit_num = 0;
  int ipc_status,r;
	message msg;
  unsigned int packet_counter=0,byte_counter=0; 
  struct packet pp;
  uint8_t pack[3];

  mouse_enable_data_reporting(); // criar esta função

  if(mouse_subscribe_int(&bit_num)!=0){return 1;}
  uint32_t irq_set=BIT(bit_num);

	while (cnt>packet_counter)
	{
    if(cnt == packet_counter){break;}
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
        mouse_ih();                       // calls mouse interrupt handler
        if(byte_counter==0){               
          pack[0]=packet_byte;             
          byte_counter++;
        }
        else if(byte_counter==1){
          pack[1]=packet_byte;
          byte_counter++;
        }
        else if(byte_counter==2){
          pack[2]=packet_byte;
          byte_counter=0;
          packet_counter++;
          get_mouse_packet(&pp,pack);
          mouse_print_packet(&pp);
        }
      }
      break;
			default:
				break;
			}
		}

	}
  mouse_unsubscribe_int();
  mouse_disable_data_reporting();
  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    /* To be completed */
    printf("%s(%u): under construction\n", __func__, idle_time);
    return 1;
}
/*
int (mouse_test_gesture)() {
    printf("%s: under construction\n", __func__);
    return 1;
}*/

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* To be completed */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
