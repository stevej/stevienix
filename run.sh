#! /bin/sh

pushd . && cd src && make && popd
. make_image.sh
qemu-system-i386 -s -cpu core2duo -cdrom stevienix.iso -vga vmware -m 128 -serial stdio -usb -usbdevice mouse -d int,in_asm,cpu_reset -D qemu.log
