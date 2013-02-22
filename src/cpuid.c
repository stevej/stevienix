#include "cpuid.h"
void cpuid () {
    u32 i;
    u32 index = 0;
    u32 regs[4];
    u32 sum;
    __asm__ __volatile__(
#if defined(__x86_64__) || defined(_M_AMD64) || defined (_M_X64)
        "pushq %%rbx     \n\t" /* save %rbx */
#else
        "pushl %%ebx     \n\t" /* save %ebx */
#endif
        "cpuid            \n\t"
        "movl %%ebx ,%[ebx]  \n\t" /* write the result into output var */
#if defined(__x86_64__) || defined(_M_AMD64) || defined (_M_X64)
        "popq %%rbx \n\t"
#else
        "popl %%ebx \n\t"
#endif
        : "=a"(regs[0]), [ebx] "=r"(regs[1]), "=c"(regs[2]), "=d"(regs[3])
        : "a"(index));

    for (i=4; i<8; i++) {
        printk("%c" ,((char *)regs)[i]);
    }
    for (i=12; i<16; i++) {
        printk("%c" ,((char *)regs)[i]);
    }
    for (i=8; i<12; i++) {
        printk("%c" ,((char *)regs)[i]);
    }
    printk("\n");
}
