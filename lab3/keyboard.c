#include <lcom/lcf.h>
#include <i8042.c>

int kbd_hid = 1;
int siCounter = 0;
uint8_t scancode[2];


int (kbc_subscribe_int) (uint8_t *bit_no) {
  *bit_no = (uint8_t) kbd_hid;

  if (sys_irqsetpolicy(KBD_IRC, (IRQ_REENABLE | IRQ_EXCLUSIVE), &kbd_hid)) {
    printf("sys_irqsetpolicy error");
    return 1;
  }
  return 0;
}

int (kbc_unsubscribe_int)() {
	if (sys_irqrmpolicy(&kbd_hid)) {
    printf("sys_irqrmpolicy error");
    return 1;
  }
  return 0;
}


int (get_kdb_scancode)(uint8_t *scancode) {
  uint8_t stat, data;
  int attemptcount = 0;
  while (attemptcount < 3) {
    attemptcount++
    util_sys_inb(KBC_SR, &stat);  
    siCounter++;
    if (stat & KBC_OBF) {  	// Check if output buffer is full
      util_sys_inb(KBC_OB, &data);  // read scancode
      siCounter++;
      if ((stat & (KBC_PARITY | KBC_TIMEOUT)) == 0) { // if either one are set to 1, it gives an error
        *scancode = data;
        return 0;
		}
      break;
    }
    tickdelay(micros_to_ticks(KBD_DELAY));
  }
  return -1;
}

uint8_t size = 0;
bool done;
bool make;


void (kbc_ih)() {
  uint8_t data;

  if (done)
    size = 0;
  get_kdb_data(&data);
  if (data & KBC_PARITY)
    make = false;
  else
    make = true;
  if (data == 0xE0) {
    scancode[0] = data;
    size++;
    done = false;
    return;
  }
  else {
    scancode[size] = data;
    size++;
    done = true;
    return;
  }
 
  return;
}
