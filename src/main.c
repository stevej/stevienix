// Borrowed from James Malloy's tutorial
#include "multiboot.h"
#include "screen.h"
#include "descriptor_tables.h"
#include "timer.h"

int main(struct multiboot *mboot_ptr) {
  init_descriptor_tables();

  screen_clear();
  screen_write("kuatos 0.1\n");

  asm volatile("int $0x3");
  asm volatile("int $0x4");

  init_timer(50); // we'll get back to our timer when it's time to timeslice.


  return 0;
}
