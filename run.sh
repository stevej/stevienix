#! /bin/sh

qemu-system-i386 -cpu pentium -kernel src/kernel.bin -m 32 -monitor stdio -d int,in_asm,cpu_reset -D qemu.log
