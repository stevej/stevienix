// Borrowed from James Malloy's tutorial
#include "multiboot.h"
#include "screen.h"
#include "descriptor_tables.h"
#include "timer.h"

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

}
