#include "gui.h"
#include "graphics.h"

static int mouse_x = SCREEN_W / 2;
static int mouse_y = SCREEN_H / 2;
static int start_pressed = 0;

static const int taskbar_h = 22;
static const int start_w = 54;

void gui_init(void) {
    gui_draw();
}

static void draw_cursor(int x, int y, u8 color) {
    for (int i = 0; i < 10; ++i) gfx_putpixel(x, y + i, color);
    for (int i = 0; i < 6; ++i) gfx_putpixel(x + i, y, color);
    gfx_putpixel(x + 1, y + 1, color);
    gfx_putpixel(x + 2, y + 2, color);
    gfx_putpixel(x + 3, y + 3, color);
}

void gui_draw(void) {
    gfx_clear(3);

    for (int y = 0; y < SCREEN_H - taskbar_h; y += 20) {
        for (int x = 0; x < SCREEN_W; x += 20) {
            gfx_rect(x, y, 1, 20, 2);
            gfx_rect(x, y, 20, 1, 2);
        }
    }

    gfx_rect(0, SCREEN_H - taskbar_h, SCREEN_W, taskbar_h, 8);
    gfx_rect(0, SCREEN_H - taskbar_h - 1, SCREEN_W, 1, 15);

    u8 start_color = start_pressed ? 12 : 10;
    gfx_rect(4, SCREEN_H - taskbar_h + 3, start_w, taskbar_h - 6, start_color);
    gfx_text(14, SCREEN_H - taskbar_h + 10, "Start", 15);

    gfx_text(70, SCREEN_H - taskbar_h + 10, "ZerO-3.0 desktop", 15);

    draw_cursor(mouse_x, mouse_y, 15);
}

void gui_mouse_move(int dx, int dy) {
    draw_cursor(mouse_x, mouse_y, 3);

    mouse_x += dx;
    mouse_y += dy;

    if (mouse_x < 0) mouse_x = 0;
    if (mouse_y < 0) mouse_y = 0;
    if (mouse_x >= SCREEN_W) mouse_x = SCREEN_W - 1;
    if (mouse_y >= SCREEN_H) mouse_y = SCREEN_H - 1;

    draw_cursor(mouse_x, mouse_y, 15);
}

void gui_mouse_click(int left_down) {
    int in_start = mouse_x >= 4 && mouse_x < 4 + start_w &&
                   mouse_y >= SCREEN_H - taskbar_h + 3 && mouse_y < SCREEN_H - 3;
    start_pressed = (left_down && in_start) ? 1 : 0;
    gui_draw();
}
