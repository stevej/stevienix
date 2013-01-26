#ifndef INITRD_H
#define INITRD_H

#include "types.h"
#include "fs.h"
#include "io.h"

typedef struct
{
    u32 nfiles; // The number of files in the ramdisk.
} initrd_header_t;

typedef struct
{
    u8 magic;     // Magic number, for error checking.
    i8 name[64];  // Filename.
    u32 offset;   // Offset in the initrd that the file starts.
    u32 length;   // Length of the file.
} initrd_file_header_t;

// Initialises the initial ramdisk. It gets passed the address of the multiboot module,
// and returns a completed filesystem node.
fs_node_t *initialise_initrd(u32 location);

#endif
