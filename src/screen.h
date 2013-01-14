#ifndef SCREEN_H
#define SCREEN_H

// Write a single character out to the screen.
void screen_put(char c);

// Clear the screen to all black.
void screen_clear();

// Output a null-terminated ASCII string to the screen.
void screen_write(char *c);

#endif // SCREEN_H
