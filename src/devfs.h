#ifndef _DEVFS_H
#define _DEVFS_H

#include "fs.h"

extern fs_node_t *devfs;
// /dev/keyboard
extern fs_node_t *devfs_keyboard;

void devfs_keyboard_attach(fs_node_t *kbd);

#endif // _DEVFS_H
