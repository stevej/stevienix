#ifndef SERIAL_H
#define SERIAL_H

#include "types.h"

// TODO(stevej): allow ports other than COM1
void serial_init();
void serial_write(char * buf);

char read_serial();

#endif
