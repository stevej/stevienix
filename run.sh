#! /bin/sh

. build_floppy.sh
qemu-system-i386 -s -cpu pentium -fda floppy.img -m 48 -monitor stdio -usb -usbdevice mouse -d int,in_asm,cpu_reset -D qemu.log
