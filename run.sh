#! /bin/sh

cat stage1 stage2 pad src/kernel.bin > floppy.img
qemu-system-x86_64  -fda floppy.img -m 32 -D qemu.log -monitor stdio
