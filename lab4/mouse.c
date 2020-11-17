#include <lcom/lcf.h>
#include "i8042.h"
#include "mouse.h"


int mouse_hid=2;

int (mouse_subscribe_int) (uint8_t *bit_no) {
  *bit_no = (uint8_t) mouse_hid;

  if (sys_irqsetpolicy(MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &mouse_hid)) {
    printf("sys_irqsetpolicy error");
    return 1;
  }
  return 0;
}

int (mouse_unsubscribe_int)() {
	if (sys_irqrmpolicy(&mouse_hid)) {
    printf("sys_irqrmpolicy error");
    return 1;
  }
  return 0;
}

void(get_mouse_packet)(struct packet *pp, uint8_t pack[3]){
  // save bytes of pack in struct
  pp->bytes[0]=pack[0];
  pp->bytes[1]=pack[1];
  pp->bytes[2]=pack[2];

  if(pack[0]&RB){       // check if RB=1
    pp->rb=true;
  }
  else{
    pp->rb=false;
  }
  if(pack[0]&LB){       // check if LB=1
    pp->lb=true;
  }
  else{
    pp->lb=false;
  }
  if(pack[0]&MB){       // check if MB=1
    pp->mb=true;
  }
  else{
    pp->mb=false;
  }
  if(pack[0]&X_OVFL){       // check if X_OVFL=1
    pp->x_ov=true;
  }
  else{
    pp->x_ov=false;
  }
  if(pack[0]&Y_OVFL){       // check if Y_OVFL=1
    pp->y_ov=true;
  }
  else{
    pp->y_ov=false;

  }

// verify sign of delta_x and delta_y
if(pack[0]&MSB_X_DELTA){
  pp->delta_x=pack[1]-256;                // MSB_X_DELTA is negative so delta_x is the complement
}
else{
  pp->delta_x=pack[1];
}

if(pack[0]&MSB_Y_DELTA){
  pp->delta_y=pack[2]-256;                // MSB_Y_DELTA is negative so delta_y is the complement
}
else{
  pp->delta_y=pack[2];
}
}

void (mouse_ih)(){
  uint8_t data,stat;
  unsigned int attempts=0;
  while(attempts<3){
    attempts++;
    if(util_sys_inb(KBC_SR,&stat)!=0){printf("sys_inb error in mouse_ih()");}
    if(stat&KBC_AUX){ // checks if data comes from mouse
      if (stat & KBC_OBF) {   // checks if there are any errors in status register
        if ((stat & (KBC_PARITY | KBC_TIMEOUT))== 0) {
        if(util_sys_inb(KBC_OB,&data)!=0){printf("sys_inb error in mouse_ih()");}
        packet_byte = data;     // saves data in variable packet_byte
        break;
        }
      }
    }
    tickdelay(micros_to_ticks(KBD_DELAY));
  }
}

int mouse_disable_data_reporting(){
  uint8_t stat,data;
  unsigned int attempts=0;
  
  if(sys_outb(KBC_CR,WRT_BYTE_MOUSE)){return 1;} // 

  while(attempts<3){
    attempts++;
    
    if(util_sys_inb(KBC_CR,&stat)){return 1;} // checks if input buffer is full
    if((stat&KBC_IB)==0){
    if(sys_outb(KBC_IB,DISABLE)){return 1;}   
    }
    
    if(util_sys_inb(KBC_OB,&data)){return 1;} // reads the acknowledgment byte

    if(data == ERROR){ // Error detected
      printf("ERROR in mouse_disable_data_reporting");
      return 1;}
    else if(data == ACK){ // if ACK everything's OK
      return 0;

    }
  }
  return 1;
}
