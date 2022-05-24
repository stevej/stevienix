// Borrowed from James Malloy's tutorial
#include "cpudet.h"
#include "descriptor_tables.h"
#include "fs.h"
#include "initrd.h"
#include "keyboard.h"
#include "kheap.h"
#include "multiboot.h"
#include "paging.h"
#include "pci.h"
#include "printk.h"
#include "screen.h"
#include "serial.h"
#include "startup.h"
#include "syscall.h"
#include "task.h"
#include "timer.h"

// void ls(fs_node_t *fs_root);

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
  __asm__ volatile("sti");
  initialise_syscalls();
  init_timer(50);

  initialise_paging();
  printk("begin tasking\n");
  initialise_tasking();

  extern u32 *initrd[];
  // Initialise the initial ramdisk, and set it as the filesystem root.
  fs_root = initialise_initrd(*initrd);
  printk("running ls on the initrd\n");
  // ls(fs_root);

  switch_to_user_mode();
  syscall_screen_write("we are now in user mode\n");
  syscall_test_noop();

  return 0;
}

// List the files in the directory
// TODO: move this into userspace once our libc is up.
/*
void ls(fs_node_t *fs_root)
{
  int i = 0;
  fs_node_t *node;

  while ((node = readdir_fs(fs_root, i)) != 0)
  {
    fs_node_t *fsnode = finddir_fs(fs_root, node->name);

    if ((fsnode->flags & 0x7) == FS_DIRECTORY)
    {
      printk("_\t[%s]\n", node->name);
    }
    else
    {
      char buffer[256];
      uint32_t size = read_fs(fsnode, 0, 256, buffer);
      printk("%d\t%s\n", size, node->name);
    }
    i++;
  }
}*/
