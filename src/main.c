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

u32 initial_esp;

void kmain(u32 initial_stack) {
  initial_esp = initial_stack;

  initialize_screen();
  //screen_write("rockville 0.1\n");
  //screen_write("initial_stack: ");
  //screen_write_hex(initial_stack);
  //screen_write("\n"); // TODO(stevej): fudge, we really need vsprintf

  init_descriptor_tables();
  asm volatile("sti");
  init_timer(50);

  initialise_paging();
  //screen_write("start tasking\n");
  initialise_tasking();

  extern task_t *current_task;
  if (current_task) {
    /*
    screen_write("current_task id:");
    screen_write_hex(current_task->id);
    screen_write("\n");*/
  } else {
    //screen_write("no current_task defined yet");
  }
  /*
  screen_write("getpid(): ");
  screen_write_hex(getpid());
  screen_write("\n");
  */

  //screen_write("Let's page!\n");

  //u32 *ptr = (u32*)0xA0000000;
  //u32 do_page_fault = *ptr;
  extern char initrd[];

  /*
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


  screen_write("initrd location:");
  screen_write_hex(initrd);
  screen_write("\n");
  */

  // Initialise the initial ramdisk, and set it as the filesystem root.
  fs_root = initialise_initrd(initrd);

// Create a new process in a new address space which is a clone of this.
  int ret = fork();

  screen_write("fork() returned ");
  screen_write_hex(ret);
  screen_write(", and getpid() returned ");
  screen_write_hex(getpid());
  screen_write("\n============================================================================\n");

  // The next section of code is not reentrant so make sure we aren't interrupted during.
  asm volatile("cli");


  // list the contents of /
  int i = 0;
  struct dirent *node = 0;
  while ( (node = readdir_fs(fs_root, i)) != 0) {
    screen_write("Found file ");
    screen_write(node->name);
    fs_node_t *fsnode = finddir_fs(fs_root, node->name);

    if ((fsnode->flags & 0x7) == FS_DIRECTORY) {
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
  //sys_exit(0);
  asm volatile("sti");
}
