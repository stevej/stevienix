#include "io.h"

// Write a byte out to the specified port.
void outb(u16 port, u8 value) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

// FIXME(stevej): check inb size
u8 inb(u16 port) {
   u8 ret;
   asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

// FIXME(stevej): check inw size
u16 inw(u16 port) {
   u16 ret;
   asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}
