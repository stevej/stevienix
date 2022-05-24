# Makefile for JamesM's kernel tutorials.
# The C and C++ rules are already setup by default.
# The only one that needs changing is the assembler
# rule, as we use nasm instead of GNU as.
KERNEL=boot.o cpudet.o devfs.o main.o io.o descriptor_tables.o isr.o interrupt.o gdt_flush.o screen.o timer.o paging.o panic.o kheap.o keyboard.o ordered_array.o initrd.o fs.o initrd_loader.o process.o task.o serial.o syscall.o pci.o port.o printk.o video.o ring_buffer.o

CC=i686-elf-gcc
LD=i686-elf-ld
NM=i686-elf-nm
CFLAGS=-nostdlib -Wall -Wextra -std=c11 -pedantic -Wshadow -Wpointer-arith -Wcast-align -Wconversion -ffreestanding -DKERNEL_H
LDFLAGS=-Tlink.ld
ASFLAGS=-felf
NASM=nasm

KERNEL_TARGET=kernel.elf
KERNEL_MAP=kernel.map

all: $(KERNEL) link map

clean:
	-rm *.o $(KERNEL_TARGET) TAGS $(KERNEL_MAP)

link:
	$(LD) $(LDFLAGS) -o $(KERNEL_TARGET) $(KERNEL)

.s.o:
	$(NASM) $(ASFLAGS) $<

map:
	$(NM) -t x $(KERNEL_TARGET) | grep -f kernel.symbols > $(KERNEL_MAP)
