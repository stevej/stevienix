From GRUB, here's how to boot:

Use the `kernel` command to tell grub where the kernel is located in
memory (beginning and length), then run `boot`


> grub> kernel 200+1024
>
>    [Multiboot-elf, blah blah blah]
>
> grub> boot


# How to burn this to a USB thumb drive on OS X (Mavericks)

From `Terminal` run `df -k`, look for your disk in `/Volumes`, you are looking for its location in `/disk/*`. Let's assume it's `/dev/disk3` which is one of the USB slots on a 2012 MacBook Pro retina.

Use diskutil to unmount the disk

`diskutil unmountDisk /dev/disk3`

and `dd` to overwrite the contents with our boot floppy image.

`sudo dd if=floppy.img of=/dev/disk3`

I'm able to boot this on an old Thinkpad t42p from 2004.


# Thanks
Thanks to the following projects for code that inspired me (or that I
just plain borrowed):

* [Nick Johnson's Rhombus OS](https://github.com/nickbjohnson4224/rhombus)
* [Kevin Lange's とあるOS](https://github.com/klange/toaruos/)

Thanks to the hobbyist community for making lots of great tutorials

* [Skelix OS tutorial](http://skelix.net/skelixos/index_en.html)
* [Bran's Kernel Tutorial](http://www.osdever.net/bkerndev/Docs/gettingstarted.htm)
* [Fogus' collection of OSDev links](http://fogus.me/thunks/osdev.html)
* [OSDev wiki](http://wiki.osdev.org/Main_Page)

Books that helped me:

* [Lion's Book](http://www.amazon.com/Lions-Commentary-Unix-John/dp/1573980137)
* [BeFS](http://www.amazon.com/Practical-System-Design-Dominic-Giampaolo-ebook)
* [Linkers and Loaders](http://www.amazon.com/Linkers-Kaufmann-Software-Engineering-Programming)
* [The Indispensable PC Hardware Book](http://www.amazon.com/The-Indispensable-Hardware-Book-Edition)
* [Design of OS/2](http://www.amazon.com/Design-OS-Harvey-M-Deitel/dp/0201548895)

