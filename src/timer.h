// timer.h -- Defines the interface for all PIT-related functions.
// Written for JamesM's kernel development tutorials.

#ifndef _TIMER_H
#define _TIMER_H

#include "task.h"
#include "types.h"

void init_timer(u32 frequency);

#endif // _TIMER_H
