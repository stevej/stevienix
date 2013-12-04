#include "keyboard.h"

void keyboard_callback(registers_t *regs) {
  while(inb(PS2_COMMAND_PORT) & 2) {}
  char scan_code = inb(PS2_DATA_PORT);
  // TODO: send back RESEND if something was wrong.
  irq_ack(IRQ1);
  printk("scan_code found: %x\n", scan_code);
  // TODO: place scan_code on port for qwerty_keyboard_server
}

bool keyboard_selftest() {
  outb(PS2_DATA_PORT, 0xAA);
  return inb(PS2_COMMAND_PORT) == 0x55;
}

void keyboard_reset() {
  // Resetting the PS/2 keyboard.
  u8 tmp = inb(0x61);
  outb(0x61, tmp | 0x80);
  outb(0x61, tmp & 0x7F);
  inb(PS2_COMMAND_PORT);
}

void keyboard_install() {
  register_interrupt_handler(IRQ1, &keyboard_callback);

  printk("keyboard selftest good: %b\n", keyboard_selftest);
  keyboard_reset();
}
