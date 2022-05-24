#ifndef _PCI_H
#define _PCI_H

#include "kheap.h"
#include "screen.h"
#include "types.h"

typedef struct pci_header {
  u16 vendor_id;
  u32 device_id;
  u16 command;
  u16 status;
  u8 revision_id;
  u8 prog_if;
  u8 subclass;
  u8 class_code;
  u8 cache_line_size;
  u8 latency_timer;
  u8 header_type;
  u8 bist;
  u32 bar[6];
  u32 cardbus_cis_ptr;
  u16 subsystem_vendor_id;
  u16 subsystem_id;
  u32 rom_base_address;
  u8 capabilities;
  u8 reserved0[3];
  u32 reserved1;
  u8 interrupt_line;
  u8 interrupt_pin;
  u8 min_grant;
  u8 max_latency;
} pci_header;

void scan_pci_bus();

#endif // _PCI_H
