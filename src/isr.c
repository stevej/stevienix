//
// isr.c -- High level interrupt service routines and interrupt request handlers.
// Part of this code is modified from Bran's kernel development tutorials.
// Rewritten for JamesM's kernel development tutorials.
//

#include "types.h"
#include "isr.h"
#include "screen.h"

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs) {
   screen_write("recieved interrupt: ");
   screen_write_dec(regs.int_no);
   screen_put('\n');
}
