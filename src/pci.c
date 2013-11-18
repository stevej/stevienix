#include "pci.h"
#include "printk.h"
#include "io.h"

u32 detect_bus() {
  return ind(0xCFC) != 0;
}

void scan_pci_bus() {
  printk("PCI bus found: %d\n", detect_bus());
}

