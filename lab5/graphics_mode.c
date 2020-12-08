#include "graphics_mode.h"




void*(vg_init)(uint16_t	mode){ 
    if(vbe_int_10(mode)){        
        printf("vbe_int_10 ERROR");
        }
    
    if(vbe_get_mode_info(mode, &vmi_p)!=0){
        printf("vbe_get_mode_info ERROR");
    }

    XRes = vmi_p.XResolution;
    YRes = vmi_p.YResolution;  
    bits_per_pixel =  vmi_p.BitsPerPixel;
   
    int r;
    struct minix_mem_range mr; /*physical memory range*/
    unsigned int vram_base;  /*VRAM’s physical addresss*/
    unsigned int vram_size;  /*VRAM’s size, but you can usethe frame-buffer size, instead*/
    
    vram_base = vmi_p.PhysBasePtr;
    vram_size = (XRes * YRes) * (bits_per_pixel/8);


    void*video_mem;         /*frame-buffer VM address*/
    /*Allow memory mapping*/
    
    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;
    
    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);/*Map memory*/
        }
    video_mem = vm_map_phys(SELF, (void*)mr.mr_base, vram_size);
    if(video_mem == MAP_FAILED){
        panic("couldn’t map video memory");	
    }
    return NULL;
}


int vbe_int_10(uint16_t mode){
    reg86_t r;
    memset(&r,0,sizeof(r));
    r.ax = VBE_CALL; // VBE call, function 02 -- set VBE mode
    r.bx = LINEAR_FRAME_BUFFER|mode; // set bit 14: linear framebuffer
    r.intno = 0x10;
    if(sys_int86(&r)){
        printf("set_vbe_mode: sys_int86() failed \n");
        return 1;
    }
    return 0;
}

int draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
  if (x >= vmi_p.XResolution)
    return 0;
  memcpy(video_mem + (x + vmi_p.XResolution * y) * ((vmi_p.BitsPerPixel + 7) / 8), &color, ((vmi_p.BitsPerPixel + 7) / 8));
  return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  int i;
  for (i = 0; i < len; i++) {
    draw_pixel(x + i, y, color);
    printf("printh:%d,%d\n", x + i, y);
  }
  return 0;
}

int(vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  int i;
  for (i = 0; i < len; i++) {
    draw_pixel(x, y + i, color);
    printf("printv:%d,%d\n", x, y + i);
  }
  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      draw_pixel(x + j, y + i, color);
    }
  }
  return 0;
}