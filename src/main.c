#include "multiboot.h"
#include "screen.h"
#include "descriptor_tables.h"

int main(struct multiboot *mboot_ptr) {
  init_descriptor_tables();

  screen_clear();
  screen_write("kuatos 0.1\n");

  asm volatile ("int $0x3");
  asm volatile ("int $0x4");

  return 0x0BADF00D;
}
