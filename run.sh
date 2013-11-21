#! /bin/sh

. build_floppy.sh
qemu-system-i386 -s -cpu core2duo -fda floppy.img -m 48 -serial stdio -usb -usbdevice mouse -d int,in_asm,cpu_reset -D qemu.log
