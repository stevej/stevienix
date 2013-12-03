# Makefile for JamesM's kernel tutorials.
# The C and C++ rules are already setup by default.
# The only one that needs changing is the assembler
# rule, as we use nasm instead of GNU as.
KERNEL=boot.o cpudet.o main.o io.o descriptor_tables.o isr.o interrupt.o gdt_flush.o screen.o timer.o paging.o panic.o kheap.o ordered_array.o initrd.o fs.o initrd_loader.o process.o task.o serial.o syscall.o pci.o port.o printk.o video.o

CC=i586-elf-gcc
LD=i586-elf-ld
NM=i586-elf-nm
CFLAGS=-nostdlib -Wall -Wextra -std=gnu99 -pedantic -Wshadow -Wpointer-arith -Wcast-align -Wconversion -ffreestanding
LDFLAGS=-Tlink.ld
ASFLAGS=-felf
NASM=nasm

KERNEL_TARGET=kernel.elf
KERNEL_MAP=kernel.map

all: $(KERNEL) link tags map

clean:
	-rm *.o $(KERNEL_TARGET) TAGS $(KERNEL_MAP)

link:
	$(LD) $(LDFLAGS) -o $(KERNEL_TARGET) $(KERNEL)

.s.o:
	$(NASM) $(ASFLAGS) $<

tags:
	etags *.c *.h

map:
	$(NM) -t x $(KERNEL_TARGET) | grep -f kernel.symbols > $(KERNEL_MAP)
