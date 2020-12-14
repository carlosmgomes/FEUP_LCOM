#include <lcom/lcf.h>
#include <i8042.h>
#include <keyboard.h>


int kbd_hid=1;

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
    attemptcount++;
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

void (kbc_ih)() {
  uint8_t data;

  if (kbd_done)
    size = 0;
  get_kdb_scancode(&data);
  if (data & KBC_PARITY)
    make = false;
  else
    make = true;
  if (data == 0xE0) {
    scancode_byte[0] = data;
    size++;
    kbd_done = false;
    return;
  }
  else {
    scancode_byte[size] = data;
    scancode=scancode_byte[size];
    size++;
    kbd_done = true;
    return;
  }
 
  return;
}
