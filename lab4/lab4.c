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

  mouse_enable_data(); // criar esta função

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
          pack[0]=packet_byte;           // first byte goes to packet[0]
          byte_counter++;                // increase byte_counter
        }
        else if(byte_counter==1){
          pack[1]=packet_byte;           // second byte goes to packet[1]
          byte_counter++;                // increase byte_counter
        } 
        else if(byte_counter==2){        
          pack[2]=packet_byte;           // third byte goes to packet[2] 
          byte_counter=0;                // restarts byte_counter
          packet_counter++;              // increase packet_counter to compare with cnt
          get_mouse_packet(&pp,pack);    // after reading 3 bytes
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

extern unsigned int counter;

int (mouse_test_async)(uint8_t idle_time) {
  int ipc_status, r;
	message msg;
	uint8_t bit_no = 12;
	uint8_t bit_no2 = 0;
	uint32_t mirq_set, tirq_set;
	uint8_t elapsedTime = 0, byteNumber = 0, bytes[3];
	struct packet pp;

	mouse_enable_data_reporting();
	mouse_subscribe_int(&bit_no);
	mirq_set = (uint32_t)(BIT(bit_no));
	timer_subscribe_int(&bit_no2);
	tirq_set = (uint32_t)(BIT(bit_no2));

	while (elapsedTime < idle_time) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failes with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.m_notify.interrupts & mirq_set) {
					mouse_ih();
					bytes[byteNumber] = packet_byte;
					byteNumber++;
					counter = 0;
					if (byteNumber == 3) {
            get_mouse_packet(&pp,bytes);
						mouse_print_packet(&pp);
						byteNumber = 0;
					}
				}
				if (msg.m_notify.interrupts & tirq_set) {
					timer_int_handler();
					if (counter % sys_hz() == 0)
						elapsedTime++;
				}
			default:
				break;
			}
		}
	}

	mouse_unsubscribe_int();
	timer_unsubscribe_int();
  	mouse_disable_data_reporting();
	return 0;
}


int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
  unsigned int packet_counter=0,byte_counter=0; 
  struct packet pp;
  uint8_t pack[3];

while (cnt>packet_counter){
mouse_ih();                       // calls mouse interrupt handler
        if(byte_counter==0){               
          pack[0]=packet_byte;           // first byte goes to packet[0]
          byte_counter++;                // increase byte_counter
        }
        else if(byte_counter==1){
          pack[1]=packet_byte;           // second byte goes to packet[1]
          byte_counter++;                // increase byte_counter
        } 
        else if(byte_counter==2){        
          pack[2]=packet_byte;           // third byte goes to packet[2] 
          byte_counter=0;                // restarts byte_counter
          packet_counter++;              // increase packet_counter to compare with cnt
          get_mouse_packet(&pp,pack);    // after reading 3 bytes
          mouse_print_packet(&pp);
        }
}
  mouse_enable_data();
  mouse_disable_data_reporting();
  minix_default_cmd_byte();
  return 0;
}

uint8_t bytes[3];


int (mouse_test_gesture)(uint8_t xlen, uint8_t tolerance) {
  bool done = false;
  uint8_t bit_no = 0;
  int ipc_status,r;
  message msg;
  mirq_set = (uint32_t)(BIT(bit_no));
  bool ReadSecond = false, ReadThird = false;
  uint8_t pack[3];

  mouse_enable_data();
  if (mouse_subscribe_int(&bit_no)!=0) return 1;

  while(!done){
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
      }
      if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: 
            if (msg.m_notify.interrupts & irq_set) {
              mouse_ih();
              if (!ReadSecond && !ReadThird && (packet_byte & BIT(3))){
                bytes[0] = packet_byte;
                ReadSecond = true;
              }
              else if (ReadSecond) {
                bytes[1] = packet_byte;
                ReadThird = true;
                ReadSecond = false;
              }
              else if (ReadThird) {
                bytes[2] = packet_byte;
                ReadThird = false;
                for(unsigned int i = 0; i < 3; i++)
                  pack[i] = bytes[i];
                get_mouse_packet(&pp,pack);
                mouse_print_packet(&pp);
                done = draw_handler(xlen, tolerance, &pp);
              }
              else
                continue;
            }
            break;
          default:
            break; 
        }
      }
  }

  if (mouse_unsubscribe_int()!=0) return 1;
  if (mouse_disable_data_report() != 0) return 1;
  
  return 0;
}