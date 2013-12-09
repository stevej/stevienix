#include "devfs.h"

void devfs_keyboard_attach(fs_node_t * kbd) {
  devfs_keyboard = kbd;
}
