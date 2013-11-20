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

void print_pci(u16 bus, u16 device, u16 func) {
  u16 vendor_code = pci_config_read(bus, device, func, 0);

  if (vendor_code != 0xFFFF) {
    // Valid device found
    printk("%d:%d:%d ", bus, device, func);
    u16 device_code = pci_config_read(bus, device, func, 2);
    print_pci_entry(lookup_vendor(vendor_code), lookup_device(vendor_code, device_code));
  }
}


void scan_pci_bus() {
  printk("PCI bus found! enumerating bus for devices\n");
  for (u16 bus = 0; bus < 256; bus++) {
    for (u16 device = 0; device < 32; device++) {
      for (u8 func = 0; func < 8; func++) {
        print_pci(bus, device, func);
      }
    }
  }
}
