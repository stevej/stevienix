#ifndef IO_H
#define IO_H

#include "types.h"

extern void outb(u16 port, u8 value);
extern void outw(u16 port, u16 value);
extern void outd(u16 port, u32 value);

extern u8  inb(u16 port);
extern u16 inw(u16 port);
extern u32 ind(u16 port);


//void memset(u8 *dest, u8 val, u32 len);
//void memcpy(u8 *dest, const u8 *src, u32 len);

//char *strcpy(char *dest, const char *src);
//char *strcat(char *dest, const char *src);


#endif
