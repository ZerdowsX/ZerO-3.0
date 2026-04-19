#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "types.h"

#define SCREEN_W 320
#define SCREEN_H 200

void gfx_clear(u8 color);
void gfx_putpixel(i32 x, i32 y, u8 color);
void gfx_rect(i32 x, i32 y, i32 w, i32 h, u8 color);
void gfx_char(i32 x, i32 y, char c, u8 color);
void gfx_text(i32 x, i32 y, const char* s, u8 color);

#endif
