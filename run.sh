#! /bin/sh

cat stage1 stage2 pad src/kernel.bin > floppy.img
qemu-system-i386 -cpu 486 -fda floppy.img -m 32 -monitor stdio -d int,in_asm,cpu_reset -D qemu.log
