# Makefile for JamesM's kernel tutorials.
# The C and C++ rules are already setup by default.
# The only one that needs changing is the assembler
# rule, as we use nasm instead of GNU as.
SOURCES=boot.o main.o io.o descriptor_tables.o isr.o interrupt.o gdt_flush.o screen.o timer.o paging.o panic.o kheap.o ordered_array.o initrd.o fs.o initrd_loader.o process.o task.o

CC=i586-elf-gcc
LD=i586-elf-ld
CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra -std=gnu99 -pedantic
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

all: $(SOURCES) link

clean:
	-rm *.o kernel.bin

link:
	$(LD) $(LDFLAGS) -o kernel.bin $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<
