// timer.c -- Initialises the PIT, and handles clock updates.
// Written for JamesM's kernel development tutorials.

#include "timer.h"
#include "io.h"
#include "isr.h"
#include "screen.h"

u32 tick = 0;

static void timer_callback(registers_t *regs) {
  if (!regs) {
    screen_writeln("regs not passed into timer_callback");
  }
  tick++;
  switch_task();
}

// Precondition: sti must be called on the processor. Currently boot.s handles
// this.
void init_timer(u32 frequency) {
  __asm__ volatile("cli");
  // Firstly, register our timer callback.
  register_interrupt_handler(IRQ0, &timer_callback);

  // The value we send to the PIT is the value to divide it's input clock
  // (1193180 Hz) by, to get our required frequency. Important to note is
  // that the divisor must be small enough to fit into 16-bits.
  u32 divisor = 1193180 / frequency;

  // Send the command byte.
  outb(0x43, 0x36);

  // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
  u8 l = (u8)(divisor & 0xFF);
  u8 h = (u8)((divisor >> 8) & 0xFF);

  // Send the frequency divisor.
  outb(0x40, l);
  outb(0x40, h);
  __asm__ volatile("sti");
}
