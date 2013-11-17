From GRUB, here's how to boot:

Use the `kernel` command to tell grub where the kernel is located in
memory (beginning and length), then run `boot`


> grub> kernel 200+1024
>    [Multiboot-elf, blah blah blah]
> grub> boot

Thanks to the following projects for code that inspired me (or that I
just plain borrowed):

# [Nick Johnson's Rhombus OS](https://github.com/nickbjohnson4224/rhombus)
# [Kevin Lange's とあるOS](https://github.com/klange/toaruos/)

Thanks to the hobbyist community for making lots of great tutorials

# [Skelix OS tutorial](http://skelix.net/skelixos/index_en.html)
# [Bran's Kernel Tutorial](http://www.osdever.net/bkerndev/Docs/gettingstarted.htm)
# [Fogus' collection of OSDev links](http://fogus.me/thunks/osdev.html)
# [OSDev wiki](http://wiki.osdev.org/Main_Page)

