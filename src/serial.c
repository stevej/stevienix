#include "serial.h"


#define PORT 0x3f8   /* COM1 */

void serial_init() {
  outb(PORT + 1, 0x00);    // Disable all interrupts
  outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
  outb(PORT + 0, 0x01);    // Set divisor to 1 (lo byte) 115200 baud
  outb(PORT + 1, 0x00);    //                  (hi byte)
  outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
  outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
  outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int is_transmit_empty() {
   return inb(PORT + 5) & 0x20;
}

void serial_write_char(char a) {
   while (is_transmit_empty() == 0);

   outb(PORT,a);
}

void serial_write(char * buf) {
  int i = 0;
  while(buf[i]) {
    serial_write_char(buf[i++]);
  }
}

int serial_received() {
  return inb(PORT + 5) & 1;
}

char read_serial() {
  while (serial_received() == 0);

  return inb(PORT);
}
