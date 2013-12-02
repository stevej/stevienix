#include "pci.h"
#include "printk.h"
#include "io.h"

#include "pci_vendor.h"

u32 detect_bus() {
  return inl(0xCFC) != 0;
}

/**
 * Calling with an offset of 0 returns the vendor id.
 * Calling with an offset of 2 returns the device id.
 */
u16 pci_config_read (u32 bus, u32 device, u32 func, u32 offset) {
  u32 address = ((bus << 16) | (device << 11) |
                 (func << 8) | (offset & 0xfc) | ((u32)0x80000000));

  outl(0xCF8, address);

  return (u16)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
}

u8 pci_exists(u32 bus, u32 slot, u32 func) {
  u16 vendor_code = pci_config_read(bus, slot, func, 0);
  return (vendor_code != 0xFFFF);
}

void print_pci_entry(PCI_VENTABLE vendor, PCI_DEVTABLE device) {
  printk("%x:%x %s %s\n", vendor.VenId, device.DevId, vendor.VenFull, device.ChipDesc);
}

PCI_VENTABLE lookup_vendor(u16 vendorID) {
  for (int i = 0; i < PCI_VENTABLE_LEN; i++) {
    if (PciVenTable[i].VenId == vendorID) {
      return PciVenTable[i];
    }
  }

  return (PCI_VENTABLE){vendorID, "Unknown", "Unknown"};
}

PCI_DEVTABLE lookup_device(u16 vendorID, u16 deviceID) {
  for(int i = 0; i < PCI_DEVTABLE_LEN; i++) {
    if (PciDevTable[i].VenId == vendorID &&
        PciDevTable[i].DevId == deviceID) {
      return PciDevTable[i];
    }
  }

  return (PCI_DEVTABLE){vendorID, deviceID, "Unknown", "Unknown"};
}

void print_pci_header_nice(struct pci_header *header) {
  printk("%x %x, %x", header->class_code, header->subclass, header->prog_if);
  print_pci_entry(lookup_vendor(header->vendor_id), lookup_device(header->vendor_id, header->device_id));
}

void print_pci_header_full(struct pci_header *header) {
  printk("DEVICE: vendor_id:%x device_id:%x command:%x status:%x revision_id:%x prog_if:%x subclass:%x \
class_code:%x cache_line_size:%x latency_timer:%x header_type:%x bist:%x \
bar[0]:%x bar[1]:%x bar[2]:%x bar[3]:%x bar[4]:%x bar[6]:%x cardbus_cis_ptr:%x \
subsystem_id:%x subsystem_vendor_id:%x rom_base_address:%x capabilities:%x \
interrupt_line:%x interrupt_pin:%x min_grant:%x max_latency:%x\n",
         header->vendor_id,
         header->device_id,
         header->command,
         header->status,
         header->revision_id,
         header->prog_if,
         header->subclass,
         header->class_code,
         header->cache_line_size,
         header->latency_timer,
         header->header_type,
         header->bist,
         header->bar[0],
         header->bar[1],
         header->bar[2],
         header->bar[3],
         header->bar[4],
         header->bar[5],
         header->cardbus_cis_ptr,
         header->subsystem_id,
         header->subsystem_vendor_id,
         header->rom_base_address,
         header->capabilities,
         header->interrupt_line,
         header->interrupt_pin,
         header->min_grant,
         header->max_latency
         );
}



/**
 * From the bus, slot, and function, read the PCI header data
 * into *header
 */
void pci_read_header(u32 bus, u32 slot, u32 func, struct pci_header *header) {
  u32 addr;
  u32 *header_data = (void*) header;

  addr = (bus << 16) | (slot << 11) | (func << 8) | 0x80000000;

  for (u32 i = 0; i < 15; i++) {
    outl(0xCF8, addr | (i << 2));
    header_data[i] = inl(0xCFC);
  }
}

/**
 * Does a brute-force enumeration of the PCI bus and prints a nice form of each header found.
 *
 * TODO(stevej): build a device list.
 * TODO(stevej): configure bus-to-bus devices for forwarding.
 */
void scan_pci_bus() {
  printk("PCI bus found! enumerating bus for devices\n");
  for (u16 bus = 0; bus < 256; bus++) {
    for (u16 device = 0; device < 32; device++) {
      for (u8 func = 0; func < 8; func++) {
        if (pci_exists(bus, device, func)) {
          struct pci_header *header = kmalloc(sizeof(pci_header));
          pci_read_header(bus, device, func, header);
          printk("[%x:%x:%x] ", bus, device, func);
          print_pci_header_nice(header);

          if (header->class_code == 0x2 &&
              header->subclass == 0x0) {
            printk("ETH: ");
            print_pci_header_full(header);
          } else if (header->class_code == 0x3 &&
                     header->subclass == 0x0) {
            printk("VIDEO: ");
            print_pci_header_full(header);
          }
        }
      }
    }
  }
}
