#include "io.h"
#include "types.h"

// Copy len bytes from src to dest.
void memcpy(u8 *dest, const u8 *src, u32 len) {
  const u8 *sp = (const u8 *)src;
  u8 *dp = (u8 *)dest;
  for (; len != 0; len--)
    *dp++ = *sp++;
}

// Write len copies of val into dest.
/*void memset(u8 *dest, u8 val, u32 len)
{
  u8 *temp = (u8 *)dest;
  for (; len != 0; len--)
    *temp++ = val;
}*/

void *memset(void *b, u8 val, u32 count) {
  __asm__ volatile("cld; rep stosb"
                   : "+c"(count), "+D"(b)
                   : "a"(val)
                   : "memory");
  return b;
}

// Compare two strings. Should return -1 if
// str1 < str2, 0 if they are equal or 1 otherwise.
int strcmp(char *str1, char *str2) {
  int i = 0;
  int failed = 0;
  while (str1[i] != '\0' && str2[i] != '\0') {
    if (str1[i] != str2[i]) {
      failed = 1;
      break;
    }
    i++;
  }
  // why did the loop exit?
  if ((str1[i] == '\0' && str2[i] != '\0') ||
      (str1[i] != '\0' && str2[i] == '\0'))
    failed = 1;

  return failed;
}

// Copy the NULL-terminated string src into dest, and
// return dest.
char *strcpy(char *dest, const char *src) {
  do {
    *dest++ = *src++;
  } while (*src != 0);

  return dest;
}

// Concatenate the NULL-terminated string src onto
// the end of dest, and return dest.
char *strcat(char *dest, const char *src) {
  while (*dest != 0) {
    char next = *dest++;
    if (next) {
      *dest = next;
    }
  }

  do {
    *dest++ = *src++;
  } while (*src != 0);

  return dest;
}

int strlen(char *src) {
  int i = 0;
  while (*src++)
    i++;
  return i;
}
