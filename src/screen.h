#ifndef _SCREEN_H
#define _SCREEN_H

#include "types.h"

void initialize_screen();

// Write a single character out to the screen.
void screen_put(char c);

// Clear the screen to all black.
void screen_clear();

// Output a null-terminated ASCII string to the screen.
void screen_write(char const *c);
// Outputs a null-terminated ASCII string to the screen followed by a newline.
void screen_writeln(char const *c);

void screen_write_hex(u32 n);
void screen_write_dec(u32 n);

#endif // _SCREEN_H
