#include "panic.h"
#include "screen.h"

extern void panic(const char *message, const char *file, u32 line) {
  // We encountered a massive problem and have to stop.
  asm volatile("cli"); // Disable interrupts.

  screen_write("PANIC(");
  screen_write(message);
  screen_write(") at ");
  screen_write(file);
  screen_write(":");
  screen_write_dec(line);
  screen_write("\n");
  // Halt by going into an infinite loop.
  for(;;);
}

extern void panic_assert(const char *file, u32 line, const char *desc) {
  // An assertion failed, and we have to panic.
  asm volatile("cli"); // Disable interrupts.

  screen_write("ASSERTION-FAILED(");
  screen_write(desc);
  screen_write(") at ");
  screen_write(file);
  screen_write(":");
  screen_write_dec(line);
  screen_write("\n");
  // Halt by going into an infinite loop.
  for(;;);
}
