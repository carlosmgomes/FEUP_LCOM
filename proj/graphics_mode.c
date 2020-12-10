#include "graphics_mode.h"

int(vbe_mode_info)(uint16_t mode, vbe_mode_info_t *vmi_p) {
  mmap_t mm;
  lm_alloc(sizeof(vbe_mode_info_t), &mm);
  phys_bytes buf = mm.phys;
  reg86_t r;
  memset(&r, 0, sizeof(r));
  r.ax = 0x4F01; /* VBE get mode info */
  /* translate the buffer linear address to a far pointer */
  r.es = PB2BASE(buf); /* set a segment base */
  r.di = PB2OFF(buf);  /* set the offset accordingly */
  r.cx = mode;
  r.intno = 0x10;
  if (sys_int86(&r) != OK) { /* call BIOS */
    lm_free(&mm);
    return 1;
  }
*vmi_p=*(vbe_mode_info_t*)mm.virt;
lm_free(&mm);
return 0;
}

void *(vg_init)(uint16_t mode) {
  if (vbe_mode_info(mode, &vmi_p) != 0) {
    printf("vbe_mode_info ERROR");
  }

  XRes = vmi_p.XResolution;
  YRes = vmi_p.YResolution;
  bits_per_pixel = vmi_p.BitsPerPixel;

  int r;
  struct minix_mem_range mr; /*physical memory range*/
  unsigned int vram_base;    /*VRAM’s physical addresss*/
  unsigned int vram_size;    /*VRAM’s size, but you can usethe frame-buffer size, instead*/

  vram_base = vmi_p.PhysBasePtr;
  vram_size = (XRes * YRes) * ceil((bits_per_pixel / 8));

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
    panic("sys_privctl (ADD_MEM) failed: %d\n", r); /*Map memory*/
  }
  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
  if (video_mem == MAP_FAILED) {
    panic("couldn’t map video memory");
  }
  return NULL;
}

int vbe_int_10(uint16_t mode) {
  reg86_t r;
  memset(&r, 0, sizeof(r));
  r.ax = VBE_CALL;                   // VBE call, function 02 -- set VBE mode
  r.bx = LINEAR_FRAME_BUFFER | mode; // set bit 14: linear framebuffer
  r.intno = 0x10;
  if (sys_int86(&r)) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }
  return 0;
}

int(pixel_color)(uint16_t x, uint16_t y, uint32_t color) {
  unsigned bytes_per_pixel = ceil(bits_per_pixel / 8);
  char *ptr = video_mem;

  ptr += (x + XRes * y) * (bytes_per_pixel);

  //paint pixel
  for (unsigned int i = 0; i < (bytes_per_pixel); i++) {
    *ptr = color;
    ptr++;
    color = color >> 8;
  }

  return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (int i = 0; i < len; ++i) {
    pixel_color(x + i, y, color);
  }
  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (int i = 0; i < height; ++i) {
    vg_draw_hline(x, y + i, width, color);
  }
  return 0;
}

int vg_draw_xpm(xpm_map_t xpm, uint16_t x, uint16_t y) {
  xpm_image_t img;
  uint8_t *sprite;
  unsigned int index = 0;
  sprite = xpm_load(xpm, XPM_8_8_8_8, &img);
  for (unsigned i = 0; i < img.height; ++i) {
    for (unsigned j = 0; j < img.width; ++j) {
      if (pixel_color(x + j, y + i, img.bytes[index]) != 0) {
        return 1;
      };
      index++;
    }
  }
  return 0;
}

int vg_draw_pattern(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  uint16_t width = XRes / no_rectangles;
  uint16_t height = YRes / no_rectangles;
  uint32_t color = first, green, blue, red;

  for (int row = 0; row < no_rectangles; ++row) {
    for (int col = 0; col < no_rectangles; ++col) {
      if (mode == 0x105) {
        color = (first + (row * no_rectangles + col) * step) % (1 << vmi_p.BitsPerPixel);
      }
      else {
        uint32_t r = (first >> vmi_p.RedFieldPosition) % BIT(vmi_p.RedMaskSize);
        uint32_t g = (first >> vmi_p.GreenFieldPosition) % BIT(vmi_p.GreenMaskSize);
        uint32_t b = (first >> vmi_p.BlueFieldPosition) % BIT(vmi_p.BlueMaskSize);
        red = (r + (col * step)) % (1 << vmi_p.RedMaskSize);
        green = (g + (row * step)) % (1 << vmi_p.GreenMaskSize);
        blue = (b + ((col + row) * step)) % (1 << vmi_p.BlueMaskSize);
        color = ((red << vmi_p.RedFieldPosition) | (green << vmi_p.GreenFieldPosition) | (blue << vmi_p.BlueFieldPosition));
      }
      if (vg_draw_rectangle(width * row, height * col, width, height, color) != 0) {
        printf("error in video_test_pattern");
        return 1;
      }
    }
  }
  return 0;
}
