#ifndef _IO_H
#define _IO_H

#include "types.h"

extern void outb(u16 port, int value);
extern void outw(u16 port, u16 value);
extern void outl(u16 port, u32 value);

extern u8 inb(u16 port);
extern u16 inw(u16 port);
extern u32 inl(u16 port);

void *memset(void *b, u8 val, u32 count);
void memcpy(u8 *dest, const u8 *src, u32 len);

char *strcpy(char *dest, const char *src);
// char *strcat(char *dest, const char *src);
int strlen(char *src);
int strcmp(char *str1, char *str2);

#endif // _IO_H
