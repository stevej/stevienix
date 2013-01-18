//
// isr.h -- Interface and structures for high level interrupt service routines.
// Part of this code is modified from Bran's kernel development tutorials.
// Rewritten for JamesM's kernel development tutorials.
//

#include "types.h"

typedef struct registers {
   u32 ds;                  // Data segment selector
   u32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   u32 int_no, err_code;    // Interrupt number and error code (if applicable)
   u32 eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;
