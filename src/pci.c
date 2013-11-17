#include "pci.h"
#include "printk.h"
#include "io.h"

u32 detect_bus() {
  return ind(0xCFC) != 0;
}

void scan_pci_bus() {
  /*  asm volatile ("                           \
    cli; \
    mov $0xB181, %eax; \
    int  $0x1A");
  */

  //register int i asm("ebx");
  printk("PCI bus found: %d\n", detect_bus());
  //asm volatile("sti;");
}

