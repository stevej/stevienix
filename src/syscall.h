// syscall.h -- Defines the interface for and structures relating to the syscall
// dispatch system.
//              Written for JamesM's kernel development tutorials.

#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "screen.h"

void initialise_syscalls();

#define DECL_SYSCALL0(fn) int syscall_##fn();
#define DECL_SYSCALL1(fn, p1) int syscall_##fn(p1);
#define DECL_SYSCALL2(fn, p1, p2) int syscall_##fn(p1, p2);
#define DECL_SYSCALL3(fn, p1, p2, p3) int syscall_##fn(p1, p2, p3);
#define DECL_SYSCALL4(fn, p1, p2, p3, p4) int syscall_##fn(p1, p2, p3, p4);
#define DECL_SYSCALL5(fn, p1, p2, p3, p4, p5)                                  \
  int syscall_##fn(p1, p2, p3, p4, p5);

#define DEFN_SYSCALL0(fn, num)                                                 \
  int syscall_##fn() {                                                         \
    int a;                                                                     \
    __asm__ volatile("int $0x80" : "=a"(a) : "0"(num));                        \
    return a;                                                                  \
  }

#define DEFN_SYSCALL1(fn, num, P1)                                             \
  int syscall_##fn(P1 p1) {                                                    \
    int a;                                                                     \
    __asm__ volatile("int $0x80" : "=a"(a) : "0"(num), "b"((int)p1));          \
    return a;                                                                  \
  }

#define DEFN_SYSCALL2(fn, num, P1, P2)                                         \
  int syscall_##fn(P1 p1, P2 p2) {                                             \
    int a;                                                                     \
    __asm__ volatile("int $0x80"                                               \
                     : "=a"(a)                                                 \
                     : "0"(num), "b"((int)p1), "c"((int)p2));                  \
    return a;                                                                  \
  }

#define DEFN_SYSCALL3(fn, num, P1, P2, P3)                                     \
  int syscall_##fn(P1 p1, P2 p2, P3 p3) {                                      \
    int a;                                                                     \
    __asm__ volatile("int $0x80"                                               \
                     : "=a"(a)                                                 \
                     : "0"(num), "b"((int)p1), "c"((int)p2), "d"((int)p3));    \
    return a;                                                                  \
  }

#define DEFN_SYSCALL4(fn, num, P1, P2, P3, P4)                                 \
  int syscall_##fn(P1 p1, P2 p2, P3 p3, P4 p4) {                               \
    int a;                                                                     \
    __asm__ volatile("int $0x80"                                               \
                     : "=a"(a)                                                 \
                     : "0"(num), "b"((int)p1), "c"((int)p2), "d"((int)p3),     \
                       "S"((int)p4));                                          \
    return a;                                                                  \
  }

#define DEFN_SYSCALL5(fn, num)                                                 \
  int syscall_##fn(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) {                        \
    int a;                                                                     \
    __asm__ volatile("int $0x80"                                               \
                     : "=a"(a)                                                 \
                     : "0"(num), "b"((int)p1), "c"((int)p2), "d"((int)p3),     \
                       "S"((int)p4), "D"((int)p5));                            \
    return a;                                                                  \
  }

extern void test_noop(void);

DECL_SYSCALL1(screen_write, const char *)
DECL_SYSCALL1(screen_write_hex, u32)
DECL_SYSCALL1(screen_write_dec, u32)
DECL_SYSCALL0(test_noop)

#endif // _SYSCALL_H
