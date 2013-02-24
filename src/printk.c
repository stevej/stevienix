#include "printk.h"
#include "screen.h"

/**
 * kernel-safe sprintk, does no dynamic allocation.
 *
 * Outputs `fmt` with directives filled in from `args` into `out`,
 * stopping at `out_length`.
 *
 * Format directives supported:
 *  - %s char *
 *  - %d int (not supported)
 *  - %x int as hex (not supported)
 *  - %p pointer (prefixed with ->) (not supported)
 *  - %% literal %
 */
void sprintk(char *out, size_t out_length, const char *fmt, va_list args) {
  int out_idx = 0;
  char *s; // space for a va_arg that's a char *

  for(int i = 0; fmt[i] != '\0'; i++) {
    char to_check = fmt[i];
    if (to_check == '%' && fmt[i+1] != '\0') { // attempt lookahead
      to_check = fmt[++i]; // jump-ahead one.
    } else {
      out[out_idx++] = to_check;
      continue;
    }

    // TODO: check that we have enough room left in out

    switch (to_check) {
    case 's':
      // TODO: check that va_args is really char*
      s = (char *)va_arg(args, char *);
      while (*s) {
        out[out_idx++] = *s++;
      }
      break;
    case 'd': // int
      out[out_idx++] = '#';
      continue;
    case 'c': // char
      out[out_idx++] = (char)va_arg(args, int);
      continue;
    case 'x': // int as hex
      continue;
    case 'p': // pointer as int
      // TODO: not tested.
      out[out_idx++] = '-';
      out[out_idx++] = '>';
      out[out_idx++] = '@'; // we haven't done this yet.
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
  screen_write(newbuf);

  va_end(args);
}
