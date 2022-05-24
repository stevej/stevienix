#ifndef _SERIAL_H
#define _SERIAL_H

#include "io.h"
#include "types.h"

// TODO(stevej): allow ports other than COM1
void serial_init();
void serial_write(char *buf);

u8 read_serial();

#endif // _SERIAL_H
