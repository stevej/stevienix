// Borrowed from James Malloy's tutorial
#include "multiboot.h"
#include "screen.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "fs.h"
#include "paging.h"
#include "kheap.h"
#include "initrd.h"

//int main(struct multiboot *mboot_ptr) {
void kmain(void) {
  initialize_screen();
  screen_write("kuatos 0.1\n");

  init_descriptor_tables();

  asm volatile("int $0x3");
  asm volatile("int $0x4");

  // init_timer(50); // we'll get back to our timer when it's time to timeslice.
  initialise_paging();
  screen_write("Let's page!\n");

  //u32 *ptr = (u32*)0xA0000000;
  //u32 do_page_fault = *ptr;

  // Initialise the screen (by clearing it)

  u32 a = kmalloc(8);
  u32 b = kmalloc(8);
  u32 c = kmalloc(8);
  screen_write("a: ");
  screen_write_hex(a);
  screen_write(", b: ");
  screen_write_hex(b);
  screen_write("\nc: ");
  screen_write_hex(c);

  kfree(c);
  kfree(b);
  u32 d = kmalloc(12);
  screen_write(", d: ");
  screen_write_hex(d);
  screen_write("\n");


  extern char initrd[];
  screen_write("initrd location:");
  screen_write_hex(initrd);
  screen_write("\n");


  // Initialise the initial ramdisk, and set it as the filesystem root.
  fs_root = initialise_initrd(initrd);

  // list the contents of /
  int i = 0;
  struct dirent *node = 0;
  while ( (node = readdir_fs(fs_root, i)) != 0) {
    screen_write("Found file ");
    screen_write(node->name);
    fs_node_t *fsnode = finddir_fs(fs_root, node->name);

    if ((fsnode->flags&0x7) == FS_DIRECTORY) {
      screen_write("\n\t(directory)\n");
    } else {
      screen_write("\n\t contents: \"");
      char buf[256];
      u32 sz = read_fs(fsnode, 0, 256, buf);
      int j;
      for (j = 0; j < sz; j++) {
        screen_put(buf[j]);
      }
      screen_write("\"\n");
    }
    i++;
  }

}
