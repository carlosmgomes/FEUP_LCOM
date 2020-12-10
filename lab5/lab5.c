// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "graphics_mode.h"
#include "i8042.h"
#include "keyboard.h"
#include "timer.h"
#include <unistd.h>

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  vg_init(mode);
  vbe_int_10(mode);
  sleep(delay);
  if (vg_exit()) {
    return 1;
  }
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {

  vg_init(mode);
  vbe_int_10(mode);
  int r, ipc_status;
  uint8_t irq_set;
  message msg;

  vg_draw_rectangle(x, y, width, height, color);

  kbc_subscribe_int(&irq_set);

  while (scancode[0] != KBD_ESC_BREAKCODE) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    // received notification
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                           // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set) // subscribed interrupt
          {
            kbc_ih(); // calls the interrupt handler
          }
          break;

        default:
          break;
      }
    }
  }
  kbc_unsubscribe_int();
  sleep(5);
  if (vg_exit()) {
    return 1;
  }

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  vg_init(mode);
  vbe_int_10(mode);
  uint8_t bit_num = 0;
  int ipc_status;
  message msg;
  int r;

  vg_draw_pattern(mode, no_rectangles, first, step);

  if (kbc_subscribe_int(&bit_num))
    return 1;
  uint32_t irq_set = BIT(bit_num);

  while (scancode[0] != KBD_ESC_BREAKCODE) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                             /* hardware interrupt notification */
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

  if (kbc_unsubscribe_int())
    return 1;
  sleep(15);
  if (vg_exit()) {
    return 1;
  }
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

  if (vg_init(0x105) == NULL) {
    vg_exit();
    return 1;
  }
  vbe_int_10(0x105);

  xpm_image_t img;
  uint8_t *map = xpm_load(xpm, XPM_INDEXED, &img);

  if (map == NULL) {
    vg_exit();
    return 1;
  }

  draw_pixmap(map, img, x, y);

  int r, ipc_status;
  uint8_t irq_set;
  message msg;

  if (kbc_subscribe_int(&irq_set)) {
    return 1;
  }

  while (scancode[0] != KBD_ESC_BREAKCODE) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    // received notification
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                           // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set) // subscribed interrupt
          {
            kbc_ih(); // calls the interrupt handler
          }
          break;

        default:
          break;
      }
    }
  }

  kbc_unsubscribe_int();

  if (vg_exit()) {
    return 1;
  }
  return 0;
}
extern uint32_t counter;

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  vg_init(0x105);
  vbe_int_10(0x105);

  uint8_t bitn;
  uint8_t bit_num;
  int frames = 0;
  if (kbc_subscribe_int(&bit_num))
    return 1;
  uint32_t irq_set = BIT(bit_num);

  if (timer_subscribe_int(&bitn))
    return 1;
  uint32_t timer_irq_set = (uint32_t)(BIT(bitn));

  message msg;
  int r, ipc_status;
  xpm_image_t img;
  uint8_t *map = xpm_load(xpm, XPM_INDEXED, &img);

  if (map == NULL) {
    vg_exit();
    return 1;
  }

  draw_pixmap(map, img, xi, yi);
  while (scancode[0] != 0x81) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                             /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
            kbc_ih();
          }
          if (msg.m_notify.interrupts & timer_irq_set) { /* subscribed interrupt */
            timer_int_handler();

            if (counter % (sys_hz() / fr_rate) == 0) {
              if (speed >= 0) {
                vg_draw_rectangle(0, 0, XRes, YRes, 0);

                if (yi == yf) {
                  xi += speed;
                  if (xi > xf) {
                    xi = xf;
                  }
                }
                if (xi == xf) {
                  yi += speed;
                  if (yi > yf) {
                    yi = yf;
                  }
                }
                draw_pixmap(map, img, xi, yi);
              }
              else {
                frames++;

                if (frames % abs(speed) == 0) {
                  vg_draw_rectangle(0, 0, XRes, YRes, 0);
                  if (yi == yf) {
                    xi++;
                    if (xi > xf) {
                      xi = xf;
                    }
                  }
                  if (xi == xf) {
                    yi++;
                    if (yi > yf) {
                      yi = yf;
                    }
                  }

                  draw_pixmap(map, img, xi, yi);
                }
              }
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
  }

  timer_unsubscribe_int();
  kbc_unsubscribe_int();

  if (vg_exit())
    return 1;
  return 0;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
