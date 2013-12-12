#include "video.h"
#include "printk.h"

#define VBE_DISPI_IOPORT_INDEX          0x01CE
#define VBE_DISPI_IOPORT_DATA           0x01CF
#define VBE_DISPI_INDEX_ID              0x0
#define VBE_DISPI_INDEX_XRES            0x1
#define VBE_DISPI_INDEX_YRES            0x2
#define VBE_DISPI_INDEX_BPP             0x3
#define VBE_DISPI_INDEX_ENABLE          0x4
#define VBE_DISPI_INDEX_BANK            0x5
#define VBE_DISPI_INDEX_VIRT_WIDTH      0x6
#define VBE_DISPI_INDEX_VIRT_HEIGHT     0x7
#define VBE_DISPI_INDEX_X_OFFSET        0x8
#define VBE_DISPI_INDEX_Y_OFFSET        0x9

#define VBE_DISPI_DISABLED              0x00
#define VBE_DISPI_ENABLED               0x01
#define VBE_DISPI_GETCAPS               0x02
#define VBE_DISPI_8BIT_DAC              0x20
#define VBE_DISPI_LFB_ENABLED           0x40
#define VBE_DISPI_NOCLEARMEM            0x80

void vbe_write(u16 index, u16 value) {
  outw(VBE_DISPI_IOPORT_INDEX, index);
  outw(VBE_DISPI_IOPORT_DATA, value);
}

u32 vbe_read(u16 index) {
  outw(VBE_DISPI_IOPORT_INDEX, index);
  return inw(VBE_DISPI_IOPORT_DATA);
}

void vbe_set(u16 xres, u16 yres, u16 bpp) {
  vbe_write(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
  vbe_write(VBE_DISPI_INDEX_XRES, xres);
  vbe_write(VBE_DISPI_INDEX_YRES, yres);
  vbe_write(VBE_DISPI_INDEX_BPP, bpp);
  vbe_write(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED | VBE_DISPI_LFB_ENABLED);
}

void video_set_resolution(u16 xres, u16 yres, u8 depth) {
  vbe_set(xres, yres, depth);
}

/* only valid for 800x600x32bpp */
static void putpixel(unsigned char* screen, int x, int y, int color) {
    unsigned where = x*4 + y*3200;
    screen[where] = color & 255;              // BLUE
    screen[where + 1] = (color >> 8) & 255;   // GREEN
    screen[where + 2] = (color >> 16) & 255;  // RED
}

void draw_test_pattern() {
  //printk("LFB enabled: %x\n", vbe_read(VBE_DISPI_LFB_ENABLED));
  //putpixel((unsigned char *)0xB8000, 400, 300, 0x000004);
}
