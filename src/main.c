// Borrowed from James Malloy's tutorial
#include "multiboot.h"
#include "screen.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "fs.h"
#include "paging.h"
#include "kheap.h"
#include "initrd.h"
#include "task.h"
#include "syscall.h"
#include "startup.h"
#include "printk.h"
#include "cpudet.h"
#include "serial.h"
#include "keyboard.h"

u32 initial_esp;

void detect_memory(multiboot *mboot_ptr) {
  printk("total memory: %dK\n", (mboot_ptr->mem_lower + mboot_ptr->mem_upper));
}


int main(multiboot *mboot_ptr, u32 initial_stack) {
  initial_esp = initial_stack;
  initialize_screen(mboot_ptr);
  serial_init(); // FIXME: don't assume COM1
  printk("stevienix 0.2\n");

  detect_cpu();

  detect_memory(mboot_ptr);
  scan_pci_bus();
  keyboard_install();


  init_descriptor_tables();
  asm volatile("sti");
  initialise_syscalls();
  init_timer(50);

  initialise_paging();
  printk("begin tasking\n");
  initialise_tasking();

  extern char initrd[];

  // Initialise the initial ramdisk, and set it as the filesystem root.
  fs_root = initialise_initrd(initrd);

  switch_to_user_mode();
  syscall_screen_write("we are now in user mode\n");
  syscall_test_noop();

  return 0;
}
