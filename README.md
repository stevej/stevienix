# Stevienix

My hobby Unix clone. Name credit to [@9len](https://twitter.com/9len)

# How to build this

You will need the following on a mac:

* GCC cross-compiler for building elf binaries
** [homebrew formulas](https://github.com/alexcrichton/homebrew-formula): i586-elf-gcc, i586-elf-binutils, i586-elf-gdb 
* mkisofs
** homebrew: cdrtools

# How to boot this

It's an ISO image, you should be able to figure it out. If you have qemu installed, you can crib the command line found in `run.sh`

# How to burn this to a USB thumb drive on OS X (Mavericks)

From `Terminal` run `df -k`, look for your disk in `/Volumes`, you are looking for its location in `/disk/*`. Let's assume it's `/dev/disk3` which is one of the USB slots on a 2012 MacBook Pro retina.

Use diskutil to unmount the disk

`diskutil unmountDisk /dev/disk3`

and `dd` to overwrite the contents with our boot floppy image.

`sudo dd if=stevienix.iso of=/dev/disk3`

I'm able to boot this on an old Thinkpad t42p from 2004.


# Thanks
Thanks to the following projects for code that inspired me (or that I
just plain borrowed, attributed appropriately):

* [Nick Johnson's Rhombus OS](https://github.com/nickbjohnson4224/rhombus)
* [Kevin Lange's とあるOS](https://github.com/klange/toaruos/)
* [VMware SVGA Device Developer Kit](http://vmware-svga.sourceforge.net)
* [L4](http://os.inf.tu-dresden.de/L4/)

Thanks to the hobbyist community for making lots of great tutorials

* [Skelix OS tutorial](http://skelix.net/skelixos/index_en.html)
* [Bran's Kernel Tutorial](http://www.osdever.net/bkerndev/Docs/gettingstarted.htm)
* [JamesM's kernel tutorial](http://www.jamesmolloy.co.uk/tutorial_html/index.html)
* [Fogus' collection of OSDev links](http://fogus.me/thunks/osdev.html)
* [OSDev wiki](http://wiki.osdev.org/Main_Page)

Books that helped me:

* [Lion's Book](http://www.amazon.com/Lions-Commentary-Unix-John/dp/1573980137)
* [BeFS](http://www.amazon.com/Practical-System-Design-Dominic-Giampaolo-ebook)
* [Linkers and Loaders](http://www.amazon.com/Linkers-Kaufmann-Software-Engineering-Programming)
* [The Indispensable PC Hardware Book](http://www.amazon.com/The-Indispensable-Hardware-Book-Edition)
* [Design of OS/2](http://www.amazon.com/Design-OS-Harvey-M-Deitel/dp/0201548895)
* [Operating System Design: The Xinu Approach](http://www.amazon.com/Operating-System-Design-Approach-Linksys/dp/143988109X)
* [PCI System Architecture](http://www.amazon.com/PCI-System-Architecture-4th-Edition/)
