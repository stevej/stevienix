#ifndef DEVFS_H
#define DEVFS_H

#include "types.h"
#include "fs.h"
#include "linked_list.h"

fs_node_t *devfs;
// /dev/keyboard
fs_node_t *devfs_keyboard;

void devfs_keyboard_attach(fs_node_t * kbd);

#endif
