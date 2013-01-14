#include "io.h"
#include "screen.h"
#include "types.h"

// The VGA framebuffer starts at 0xB8000.
u16 *video_memory = (u16 *)0xB8000;

u8 cursor_x = 0;
u8 cursor_y = 0;

static void move_cursor() {
  // The screen is 80 characters wide...
  u16 cursorLocation = cursor_y * 80 + cursor_x;
  outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
  outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
  outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
  outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}

// Scrolls the text on the screen up by one line.
static void scroll() {
  // Get a space character with the default colour attributes.
  u8 attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
  u16 blank = 0x20 /* space */ | (attributeByte << 8);

  // Row 25 is the end, this means we need to scroll up
  if(cursor_y >= 25) {
      // Move the current text chunk that makes up the screen
      // back in the buffer by a line
      int i;
      for (i = 0*80; i < 24*80; i++) {
        video_memory[i] = video_memory[i+80];
      }

      // The last line should now be blank. Do this by writing
      // 80 spaces to it.
      for (i = 24*80; i < 25*80; i++) {
        video_memory[i] = blank;
      }
      // The cursor should now be on the last line.
      cursor_y = 24;
    }
}

void screen_put(char c) {
   // The background colour is black (0), the foreground is white (15).
   u8 backColour = 0;
   u8 foreColour = 15;

   // The attribute byte is made up of two nibbles - the lower being the
   // foreground colour, and the upper the background colour.
   u8  attributeByte = (backColour << 4) | (foreColour & 0x0F);
   // The attribute byte is the top 8 bits of the word we have to send to the
   // VGA board.
   u16 attribute = attributeByte << 8;
   u16 *location;

   // Handle a backspace, by moving the cursor back one space
   if (c == 0x08 && cursor_x) {
       cursor_x--;
   } else if (c == 0x09) {
     // Handle a tab by increasing the cursor's X, but only to a point
     // where it is divisible by 8.
     cursor_x = (cursor_x+8) & ~(8-1);
   } else if (c == '\r') {   // Handle carriage return
     cursor_x = 0;
   } else if (c == '\n') { // Handle newline by moving cursor back to left and increasing the row
     cursor_x = 0;
     cursor_y++;
   } else if (c >= ' ') { // Handle any other printable character.
     location = video_memory + (cursor_y*80 + cursor_x);
     *location = c | attribute;
     cursor_x++;
   }

   // Check if we need to insert a new line because we have reached the end
   // of the screen.
   if (cursor_x >= 80) {
     cursor_x = 0;
     cursor_y ++;
   }

   // Scroll the screen if needed.
   scroll();
   // Move the hardware cursor.
   move_cursor();
}


// Clears the screen, by copying lots of spaces to the framebuffer.
void screen_clear() {
   // Make an attribute byte for the default colours
   u8 attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
   u16 blank = 0x20 /* space */ | (attributeByte << 8);

   int i;
   for (i = 0; i < 80*25; i++) {
     video_memory[i] = blank;
   }

   // Move the hardware cursor back to the start.
   cursor_x = 0;
   cursor_y = 0;
   move_cursor();
}

// Outputs a null-terminated ASCII string to the screen.
void screen_write(char *c) {
  int i = 0;
  while (c[i]) {
    screen_put(c[i++]);
  }
}
