#ifndef _KEYBOARD_H
#define _KEYBOARD_H
#include "fs.h"
#include "types.h"

#define PS2_COMMAND_PORT 0x64
#define PS2_DATA_PORT 0x60

void keyboard_install();
void keyboard_server();
fs_node_t *keyboard_create();

uint32_t keyboard_read(fs_node_t *node, uint32_t offset, uint32_t size,
                       uint8_t *buffer);

uint32_t keyboard_write(fs_node_t *node, uint32_t offset, uint32_t size,
                        uint8_t *buffer);

#endif // _KEYBOARD_H
