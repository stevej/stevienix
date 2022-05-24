#ifndef _VIDEO_H
#define _VIDEO_H

#include "io.h"
#include "types.h"

void video_set_resolution(u16 xres, u16 yres, u8 depth);
void draw_test_pattern();

#endif // _VIDEO_H
