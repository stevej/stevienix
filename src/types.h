#ifndef _TYPES_H
#define _TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * Signed integers are prefixed with i. Unsigned are prefixed with u.
 */

// FIXME: These typedefs are written for 32-bit X86.
typedef uint32_t u32;
typedef int32_t i32;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint8_t u8;
typedef int8_t i8;

// typedef unsigned int size_t;

#endif // _TYPES_H
