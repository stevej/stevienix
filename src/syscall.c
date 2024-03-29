// syscall.c -- Defines the implementation of a system call system.
//              Written for JamesM's kernel development tutorials.

#include "syscall.h"
#include "isr.h"

static void syscall_handler(registers_t *regs);

DEFN_SYSCALL1(screen_write, 0, const char *)
DEFN_SYSCALL1(screen_write_hex, 1, u32)
DEFN_SYSCALL1(screen_write_dec, 2, u32)
DEFN_SYSCALL0(test_noop, 3)

// This must be the number of elements in `syscalls`
#define SYSCALL_NO 4

static void *syscalls[SYSCALL_NO] = {&screen_write, &screen_write_hex,
                                     &screen_write_dec, &test_noop};

u32 num_syscalls = SYSCALL_NO;

void do_nothing() {}

void initialise_syscalls() {
  // Register our syscall handler.
  register_interrupt_handler(0x80, &syscall_handler);
  register_interrupt_handler(0x22, &do_nothing);
}

void test_noop() { __asm__ volatile("int $0x22"); }

void syscall_handler(registers_t *regs) {
  // Firstly, check if the requested syscall number is valid.
  // The syscall number is found in EAX.
  if (regs->eax >= num_syscalls)
    return;

  // Get the required syscall location.
  void *location = syscalls[regs->eax];

  // We don't know how many parameters the function wants, so we just
  // push them all onto the stack in the correct order. The function will
  // use all the parameters it wants, and we can pop them all back off
  // afterwards.
  u32 ret;
  __asm__ volatile(" \
      push %1; \
      push %2; \
      push %3; \
      push %4; \
      push %5; \
      call *%6; \
      pop %%ebx; \
      pop %%ebx; \
      pop %%ebx; \
      pop %%ebx; \
      pop %%ebx; \
    "
                   : "=a"(ret)
                   : "r"(regs->edi), "r"(regs->esi), "r"(regs->edx),
                     "r"(regs->ecx), "r"(regs->ebx), "r"(location));
  regs->eax = ret;
}
