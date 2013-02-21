#include "pci.h"

u32 read_AH();

void scan_pci_bus() {
  asm volatile (" \
    cli; \
    mov $0xB181, %%eax; \
    int  $0x1A; \
    sti; " : ); // empty constraints section is to workaround gcc bug

  //register int i asm("ebx");
  screen_write("PCI 2.0: ");
  screen_write_dec(read_AH());
  screen_write("\n");
}

u32 read_AH() {
  struct { u8 al; u8 ah; } status;

  asm("movb $0x12, %%ah; movb $0x34, %%al" : "=a"(status) : );
  return status.ah;
}
