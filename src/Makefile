# Makefile for JamesM's kernel tutorials.
# The C and C++ rules are already setup by default.
# The only one that needs changing is the assembler
# rule, as we use nasm instead of GNU as.
KERNEL=boot.o cpudet.o main.o io.o descriptor_tables.o isr.o interrupt.o gdt_flush.o screen.o timer.o paging.o panic.o kheap.o ordered_array.o initrd.o fs.o initrd_loader.o process.o task.o serial.o syscall.o pci.o port.o printk.o video.o

CC=i586-elf-gcc
LD=i586-elf-ld
CFLAGS=-nostdlib -nostdinc -Wall -Wextra -std=gnu99 -pedantic -Wshadow -Wpointer-arith -Wcast-align -Wconversion -ffreestanding
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

all: $(KERNEL) link tags

clean:
	-rm *.o kernel.elf TAGS

link:
	$(LD) $(LDFLAGS) -o kernel.elf $(KERNEL)

.s.o:
	nasm $(ASFLAGS) $<

tags:
	etags *.c *.h
