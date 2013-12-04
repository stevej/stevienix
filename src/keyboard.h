#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "keyboard.h"
#include "isr.h"
#include "printk.h"
#include "syscall.h"
#include "io.h"

#define PS2_COMMAND_PORT 0x64
#define PS2_DATA_PORT 0x60

void keyboard_install();

#endif
