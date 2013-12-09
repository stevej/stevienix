#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "keyboard.h"
#include "isr.h"
#include "printk.h"
#include "syscall.h"
#include "io.h"
#include "fs.h"
#include "devfs.h"

#define PS2_COMMAND_PORT 0x64
#define PS2_DATA_PORT 0x60

void keyboard_install();
void keyboard_server();
fs_node_t * keyboard_create();

uint32_t keyboard_read(fs_node_t *node,
                       uint32_t offset,
                       uint32_t size,
                       uint8_t *buffer);


uint32_t keyboard_write(fs_node_t *node,
                        uint32_t offset,
                        uint32_t size,
                        uint8_t *buffer);


#endif
