#include "printk.h"
#include "screen.h"
#include "serial.h"

/**
 * kernel-safe sprintk, does no dynamic allocation.
 *
 * Outputs `fmt` with directives filled in from `args` into `out`,
 * stopping at `out_length`.
 *
 * Format directives supported:
 *  - %s char *
 *  - %c unsigned char
 *  - %d unsigned int
 *  - %x unsigned int as hex
 *  - %p pointer (prefixed with ->)
 *  - %b bool (1 is true, all else false)
 *  - %% literal %
 */

void sprintk(char *out, size_t out_length, const char *fmt, va_list args) {
  int out_idx = 0;
  int x = 0;
  for(int i = 0; fmt[i] != '\0'; i++) {
    char to_check = fmt[i];
    if (to_check == '%' && fmt[i+1] != '\0') { // attempt lookahead
      to_check = fmt[++i]; // jump-ahead one.
    } else {
      out[out_idx++] = to_check;
      continue;
    }

    // TODO: check that we have enough room left in out
    u32 n; // a temporary
    char t[32]; // a temporary
    char * s; // space for a va_arg that's a char *

    switch (to_check) {
    case 's':
      // TODO: check that va_args is really char*
      s = (char *)va_arg(args, char *);
      while (*s) {
        out[out_idx++] = *s++;
      }
      continue;
    case 'b': // bool
      n = (int)va_arg(args, int);
      if (n) {
        out[out_idx++] = 't';
        out[out_idx++] = 'r';
        out[out_idx++] = 'u';
        out[out_idx++] = 'e';
      } else {
        out[out_idx++] = 'f';
        out[out_idx++] = 'a';
        out[out_idx++] = 'l';
        out[out_idx++] = 's';
        out[out_idx++] = 'e';
      }
      continue;
    case 'c': // char can fall-through and be treated like an unsigned decimal number.
    case 'd': // uint32_t
      n = (u32)va_arg(args, u32);
      u32_to_dec(n, t);

      x = 0;
      for (char c = t[x]; c != 0; c = t[++x]) {
        out[out_idx++] = c;
      }
      continue;
    case 'x': // int as hex
      n = (u32)va_arg(args, u32);
      u32_to_hex(n, t);

      x = 0;
      for (char c = t[x]; c != 0; c = t[++x]) {
        out[out_idx++] = c;
      }
      continue;
    case 'p': // pointer as int
      n = (u32)va_arg(args, u32);
      out[out_idx++] = '-';
      out[out_idx++] = '>';

      if (n) {
        u32_to_dec(n, t);

        x = 0;
        for (char c = t[x]; c != 0; c = t[++x]) {
          out[out_idx++] = c;
        }
      } else {
        out[out_idx++] = 'N';
        out[out_idx++] = 'U';
        out[out_idx++] = 'L';
      }
      continue;
    case '%': // escaped %
      out[out_idx++] = '%';
      continue;
    default: // treat as pointer?
      out[out_idx++] = to_check;
      break;
    }
  }
  out[out_idx] = '\0';
}

void printk(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  size_t size = 1024;
  char newbuf[1024] = {-1};
  sprintk(newbuf, size, fmt, args);
  // TODO(stevej): allow main to (de)register multiple outputs.
  screen_write(newbuf);
  serial_write(newbuf);

  va_end(args);
}

void u32_to_hex(u32 n, char out[static 32]) {
  out[0] = '0';
  out[1] = 'x';
  i8 idx = 2;

  i32 tmp;

  char noZeroes = 1;

  int i;
  for (i = 28; i > 0; i -= 4) {
    tmp = (n >> i) & 0xF;
    if (tmp == 0 && noZeroes != 0) {
      continue;
    }

    if (tmp >= 0xA) {
      noZeroes = 0;
      out[idx++] = (tmp-0xA+'a');
    } else {
      noZeroes = 0;
      out[idx++] = ( tmp+'0' );
    }
  }

  tmp = n & 0xF;
  if (tmp >= 0xA) {
    out[idx++] =  (tmp-0xA+'a');
  } else {
    out[idx++] = (tmp+'0');
  }
  out[idx++] = 0;
}

void u32_to_dec(u32 n, char out[static 32]) {
  if (n == 0) {
    out[0] = '0';
    out[1] = '\0';
    return;
  }

  i32 acc = n;
  char c[32];
  int i = 0;

  while (acc > 0) {
    c[i] = '0' + (acc % 10);
    acc /= 10;
    i++;
  }

  out[i--] = '\0';
  int j = 0;
  while (i >= 0) {
    out[i--] = c[j++];
  }
}
