//
// isr.c -- High level interrupt service routines and interrupt request
// handlers. Part of this code is modified from Bran's kernel development
// tutorials. Rewritten for JamesM's kernel development tutorials.
//

#include "isr.h"
#include "screen.h"
#include "types.h"

isr_t interrupt_handlers[256];

void register_interrupt_handler(u8 n, isr_t handler) {
  interrupt_handlers[n] = handler;
}

// This gets called from interrupt.s
void isr_handler(registers_t regs) {
  // This line is important. When the processor extends the 8-bit interrupt
  // number to a 32bit value, it sign-extends, not zero extends. So if the most
  // significant bit (0x80) is set, regs.int_no will be very large (about
  // 0xffffff80).
  u8 int_no = regs.int_no & 0xFF;
  if (interrupt_handlers[int_no] != 0) {
    isr_t handler = interrupt_handlers[int_no];
    handler(&regs);
  } else {
    screen_write("unhandled interrupt: ");
    screen_write_dec(int_no);
    screen_put('\n');
    for (;;)
      ;
  }
}

void irq_resend(u32 ir1) {
  // TODO: implement
  screen_write("unimplemented: request for irq resend: ");
  screen_write_dec(ir1);
  screen_put('\n');
}

void irq_ack(u32 irq) {
  if (irq >= 12) {
    outb(0xA0, 0x20);
  } else {
    outb(0x20, 0x20);
  }
}

// This gets called from interrupt.s
void irq_handler(registers_t regs) {
  // Send an EOI (end of interrupt) signal to the PICs.
  // If this interrupt involved the slave.
  if (regs.int_no >= 40) {
    // Send reset signal to slave.
    outb(0xA0, 0x20);
  }
  // Send reset signal to master. (As well as slave, if necessary).
  outb(0x20, 0x20);

  if (interrupt_handlers[regs.int_no] != 0) {
    isr_t handler = interrupt_handlers[regs.int_no];
    handler(&regs);
  }
}
