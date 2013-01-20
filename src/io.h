#ifndef IO_H
#define IO_H

#include "types.h"

void outb(u16 port, u8 value);
u8 inb(u16 port);
u16 inw(u16 port);

//void memset(u8 *dest, u8 val, u32 len);
//void memcpy(u8 *dest, const u8 *src, u32 len);

//char *strcpy(char *dest, const char *src);
//char *strcat(char *dest, const char *src);


#endif
