#include <lcom/lcf.h>
#include <i8042.h>
#include <keyboard.h>



int kbd_hid = 1;
uint8_t scancode;
bool error = false;

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
  if(sys_irqdisable(&kbd_hid)){
    printf("sys_irqdisable error");
    return 1;
  }
  return 0;
}


int (get_kdb_scancode)(uint8_t *scancode) {
  uint8_t stat, data;
    
  util_sys_inb(KBC_SR, &stat);  
  
  if (stat & KBC_OBF) {  	// Check if output buffer is full
    util_sys_inb(KBC_OB, &data);  // read scancode
    if ((stat & (KBC_PARITY | KBC_TIMEOUT)) == 0) { // if either one are set to 1, it gives an error
      scancode = data;
		}
    else{
      error = true;
      return
      }
   }
   else{
      error = true;
      return
      }
      error = false;
  }

void (kbc_ih)() {
  get_kdb_scancode(&data);
}
