#! /bin/sh

qemu-system-i386 -cpu pentium -kernel src/kernel.bin -m 32 -monitor stdio -d int,in_asm -D qemu.log
