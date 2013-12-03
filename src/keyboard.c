#include "keyboard.h"

void keyboard_callback(registers_t *regs) {
  syscall_screen_write("keyboard called: \n");
  while(inb(0x64) & 2) {}
  char scan_code = inb(0x60);
  // TODO: send back RESEND if something was wrong.
  irq_ack(IRQ1);
  printk("scan_code found: %x\n", scan_code);
}

void keyboard_install() {
  register_interrupt_handler(IRQ1, &keyboard_callback);

  // Resetting the PS/2 keyboard.
  u8 tmp = inb(0x61);
  outb(0x61, tmp | 0x80);
  outb(0x61, tmp & 0x7F);
  inb(0x64);
}
