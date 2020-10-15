#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

uint8_t lsb = 0, msb = 0, timer_addr=0, status;
uint16_t cicles = TIMER_FREQ/freq;
util_get_LSB(cicles,&lsb);
util_get_MSB(cicles,&msb);
if(timer_get_conf(timer,&status)!=0){
  printf("Timer_get_conf error");
  return 1;
}

if(timer==0){
  status = ((status & 0x0f)|TIMER_SEL0|TIMER_LSB_MSB);
  timer_addr = TIMER_0;
}
else if(timer==1){
  status = ((status & 0x0f)|TIMER_SEL1|TIMER_LSB_MSB);
    timer_addr = TIMER_1;

}
else if(timer==2){
   status = ((status & 0x0f)|TIMER_SEL2|TIMER_LSB_MSB);
     timer_addr = TIMER_2;

}
else{
  printf("Invalid timer\n");
  return 1;
  }
  if(sys_outb(TIMER_CTRL,status)!=0){
    return 1;
    }
  else if(sys_outb(timer_addr,msb)!=0){
    return 1;
    }
  else if(sys_outb(timer_addr,lsb)!=0){
    return 1;
    }
return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

uint16_t rbcmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
if(sys_outb(TIMER_CTRL,rbcmd)){
  printf("sys outb error\n");
  return 1;
}
uint32_t temp;

if(sys_inb(TIMER_0+timer,&temp)){
  printf("sys inb error\n");
  return 1;
}
*st = temp;
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {

union timer_status_field_val conf;

switch(field){
  case tsf_all:
    conf.byte = st;
    break;
  case tsf_initial:
    switch(tsf_initial){
      case INVAL_val:
      conf.in_mode = INVAL_val;
      break;
      case LSB_only:
      conf.in_mode = LSB_only;
      break;
      case MSB_only:
      conf.in_mode = MSB_only;
      break;
      case MSB_after_LSB:
      conf.in_mode = MSB_after_LSB;
      break;
}
  case tsf_mode:
  //uint8_t temp = ((st & (BIT(1)|BIT(2)|BIT(3)))>>1);
  switch(((st & (BIT(1)|BIT(2)|BIT(3)))>>1)){
    case 6:
    conf.count_mode = 2;
    break;
    case 7:
    conf.count_mode = 3;
    break;
    default:
    conf.count_mode = ((st & (BIT(1)|BIT(2)|BIT(3)))>>1);
    break;
  }
  case tsf_base:
  conf.bcd = (st & BIT(0));
  break;
}
timer_print_config(timer,field,conf);
   return 0;
 }
 
