#include "keyboard.h"
#include "devfs.h"
#include "io.h"
#include "isr.h"
#include "printk.h"
#include "ring_buffer.h"
#include "syscall.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

void keyboard_callback(registers_t *regs) {
  while (inb(PS2_COMMAND_PORT) & 2) {
  }
  uint8_t scan_code = inb(PS2_DATA_PORT);
  // TODO: send back RESEND if something was wrong.
  irq_ack(IRQ1);
  printk("scan_code found: %x\n", scan_code);
  // open '/dev/keyboard', write scan_code to it.
  // TODO: make this understand paths better.
  // fs_node_t *keyboard = devfs_keyboard;
  fs_node_t *keyboard = NULL; // FIXME: fix me
  if (keyboard) {
    printk("calling write_fs with %x %c\n", scan_code, scan_code);
    write_fs_byte(keyboard, 0, scan_code);
  } else {
    printk("ERROR: can't write to NULL /dev/keyboard char device.\n");
  }
}

bool keyboard_selftest() {
  outb(PS2_DATA_PORT, 0xAA);
  return inb(PS2_COMMAND_PORT) == 0x55;
}

void keyboard_reset() {
  u8 tmp = inb(0x61);
  outb(0x61, tmp | 0x80);
  outb(0x61, tmp & 0x7F);
  inb(PS2_COMMAND_PORT);
}

void keyboard_install(fs_node_t *dev_root) {
  printk("keyboard selftest good: %b\n", keyboard_selftest);
  keyboard_reset();

  fs_node_t *keyboard = keyboard_create();
  devfs_keyboard_attach(keyboard);
  register_interrupt_handler(IRQ1, &keyboard_callback);
}

// TODO: this needs to be runnable as it's own task.
void keyboard_server() {
  // todo: replace with open('/dev/keyboard');
  // fs_node_t *kbd = devfs_keyboard;
}

fs_node_t *keyboard_create() {
  fs_node_t *keyboard;
  keyboard = (fs_node_t *)kmalloc(sizeof(keyboard));
  strcpy(keyboard->name, "keyboard");
  keyboard->mask = keyboard->uid = keyboard->gid = keyboard->inode =
      keyboard->length = 0;
  keyboard->flags = FS_CHARDEVICE;
  keyboard->buffer = ring_buffer_create(8);
  // dev_root
  keyboard->write = keyboard_write;
  keyboard->read = keyboard_read;

  return keyboard;
}

uint32_t keyboard_read(fs_node_t *node, uint32_t offset, uint32_t size,
                       uint8_t *buffer) {
  // return ring_buffer_read(devfs_keyboard->buffer);
  return 0; // FIXME
}

uint32_t keyboard_write(fs_node_t *node, uint32_t offset, uint32_t size,
                        uint8_t *buffer) {
  printk("going to insert %x (%p) into the keyboard buffer\n", buffer, *buffer);
  // ring_buffer_write(devfs_keyboard->buffer, *buffer);
  return 0; // FIXME: this is busted obviously;
}
