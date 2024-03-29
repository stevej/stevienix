#ifndef _ISR_H
#define _ISR_H

//
// isr.h -- Interface and structures for high level interrupt service routines.
// Part of this code is modified from Bran's kernel development tutorials.
// Rewritten for JamesM's kernel development tutorials.
//
#include "io.h"
#include "types.h"

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

typedef struct registers {
  u32 ds;                                     // Data segment selector
  u32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
  u32 int_no, err_code; // Interrupt number and error code (if applicable)
  u32 eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;

// Enables registration of callbacks for interrupts or IRQs.
// For IRQs, to ease confusion, use the #defines above as the
// first parameter.
typedef void (*isr_t)(registers_t *);
void register_interrupt_handler(u8 n, isr_t handler);

void irq_resend(u32 irq);
void irq_ack(u32 irq);

#endif // _ISR_H
