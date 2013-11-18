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

u32 initial_esp;

void detect_memory(struct multiboot *mboot_ptr) {
  printk("total memory: %dK\n", (mboot_ptr->mem_lower + mboot_ptr->mem_upper));
}

int main(struct multiboot *mboot_ptr, u32 initial_stack) {
  initial_esp = initial_stack;
  initialize_screen(mboot_ptr);
  printk("stevienix 0.2\n");

  detect_cpu();

  detect_memory(mboot_ptr);
  scan_pci_bus();

  init_descriptor_tables();
  asm volatile("sti");
  initialise_syscalls();
  init_timer(50);

  initialise_paging();
  screen_write("start tasking\n");
  initialise_tasking();
  //printk("testing printk 1\n");

  //char newbuf[1024] = {-1};

  //sprintk(newbuf, 1024, "testing printk %c 2\n", 'a');
  //screen_write(newbuf);
  printk("testing %%c: %c\n", 'a');
  printk("testing %%s: %s\n", "hello");
  printk("testing %%d 0: %d\n", 0);
  printk("testing %%d 1: %d\n", 1);
  printk("testing %%d 911: %d\n", 911);
  printk("testing %%x 0: %x\n", 0);
  printk("testing %%x 1: %x\n", 1);
  printk("testing %%x 911: %x\n", 911);


  printk("\n");

  extern char initrd[];

  // Initialise the initial ramdisk, and set it as the filesystem root.
  fs_root = initialise_initrd(initrd);

  switch_to_user_mode();
  syscall_screen_write("stevienix 0.1 is currently in user mode\n");
  syscall_test_noop();

  return 0;
}
