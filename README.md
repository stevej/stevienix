From GRUB, here's how to boot:

Use the `kernel` command to tell grub where the kernel is located in
memory (beginning and length), then run `boot`


> grub> kernel 200+1024
>    [Multiboot-elf, blah blah blah]
> grub> boot

