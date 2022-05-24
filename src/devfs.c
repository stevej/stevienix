#include "devfs.h"

fs_node_t *devfs = NULL;
// /dev/keyboard
fs_node_t *devfs_keyboard = NULL;

void devfs_keyboard_attach(fs_node_t *kbd) { devfs_keyboard = kbd; }
