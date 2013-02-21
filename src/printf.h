#ifndef PRINTF_H
#define PRINTF_H

#include "types.h"
#include "stdarg.h"

// print into s
void sprintk(char *s, size_t size, const char *fmt, va_list args);

// print to screen
void printk(const char *fmt, ...);

#endif
