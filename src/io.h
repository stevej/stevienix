#ifndef IO_H
#define IO_H

#include "types.h"

void outb(u16 port, u8 value);
u8 inb(u16 port);
u16 inw(u16 port);

#endif
