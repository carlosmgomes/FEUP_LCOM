#include <lcom/lcf.h>

#include <stdint.h>
<<<<<<< HEAD
uint32_t counter=0; // value to count number of sys_inb calls to use in lab3(kbd_print_no_sysinb) 
=======

>>>>>>> 30c40705b060dd83a87220bf348c43b3e0ff2491
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
 *msb = val >> 8;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t temp;
  sys_inb(port, &temp);
  *value = ( uint8_t ) temp;
<<<<<<< HEAD
  counter +=1;
=======
>>>>>>> 30c40705b060dd83a87220bf348c43b3e0ff2491
  return 0;
}
