#ifndef SCREEN_H
#define SCREEN_H

#include "types.h"

// Write a single character out to the screen.
void screen_put(char c);

// Clear the screen to all black.
void screen_clear();

// Output a null-terminated ASCII string to the screen.
void screen_write(char *c);

void screen_write_hex(u32 n);
void screen_write_dec(u32 n);

#endif
