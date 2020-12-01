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
