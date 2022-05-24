#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "types.h"

#define MULTIBOOT_MAGIC 0x1BADB002
#define MULTIBOOT_EAX_MAGIC 0x2BADB002
#define MULTIBOOT_FLAG_MEM 0x001
#define MULTIBOOT_FLAG_DEVICE 0x002
#define MULTIBOOT_FLAG_CMDLINE 0x004
#define MULTIBOOT_FLAG_MODS 0x008
#define MULTIBOOT_FLAG_AOUT 0x010
#define MULTIBOOT_FLAG_ELF 0x020
#define MULTIBOOT_FLAG_MMAP 0x040
#define MULTIBOOT_FLAG_CONFIG 0x080
#define MULTIBOOT_FLAG_LOADER 0x100
#define MULTIBOOT_FLAG_APM 0x200
#define MULTIBOOT_FLAG_VBE 0x400

typedef struct multiboot {
  u32 flags;
  u32 mem_lower;
  u32 mem_upper;
  u32 boot_device;
  u32 cmdline;
  u32 mods_count;
  u32 mods_addr;
  u32 num;
  u32 size;
  u32 addr;
  u32 shndx;
  u32 mmap_length;
  u32 mmap_addr;
  u32 drives_length;
  u32 drives_addr;
  u32 config_table;
  u32 boot_loader_name;
  u32 apm_table;
  u32 vbe_control_info;
  u32 vbe_mode_info;
  u32 vbe_mode;
  u32 vbe_interface_seg;
  u32 vbe_interface_off;
  u32 vbe_interface_len;
} __attribute__((packed)) multiboot;

typedef struct {
  u16 attributes;
  u8 winA, winB;
  u16 granularity;
  u16 winsize;
  u16 segmentA, segmentB;
  u32 realFctPtr;
  u16 pitch;

  u16 Xres, Yres;
  u8 Wchar, Ychar, planes, bpp, banks;
  u8 memory_model, bank_size, image_pages;
  u8 reserved0;

  u8 red_mask, red_position;
  u8 green_mask, green_position;
  u8 blue_mask, blue_position;
  u8 rsv_mask, rsv_position;
  u8 directcolor_attributes;

  u32 physbase;
  u32 reserved1;
  u16 reserved2;
} __attribute__((packed)) vbe_info_t;

multiboot *copy_multiboot(multiboot *mboot_ptr);
void dump_multiboot(multiboot *mboot_ptr);
char *ramdisk;

#endif // _SYSTEM_H
