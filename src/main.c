#include "multiboot.h"
#include "screen.h"

int main(struct multiboot *mboot_ptr) {
  screen_clear();
  screen_write("kuatos 0.1");

  return 0x0BADF00D;
}
