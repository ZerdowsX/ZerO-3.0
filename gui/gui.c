#include "gui.h"
#include "graphics.h"

static int mouse_x = SCREEN_W / 2;
static int mouse_y = SCREEN_H / 2;
static int start_pressed = 0;
static int start_menu_open = 0;
static int mouse_ready = 0;
static int mouse_left_down = 0;
static int cursor_drawn = 0;
static int cursor_prev_x = 0;
static int cursor_prev_y = 0;
static u8 cursor_bg[8 * 12];

static const int taskbar_h = 22;
static const int start_w = 54;

void gui_init(void) {
    gui_draw();
}

static void draw_cursor(int x, int y) {
    static const char* shape[] = {
        "X.......",
        "XX......",
        "XOX.....",
        "XOOX....",
        "XOOOX...",
        "XOOOOX..",
        "XOOOOOX.",
        "XOOOOOOX",
        "XOOOOX..",
        "XOOX....",
        "XX......",
        "X......."
    };

    for (int row = 0; row < 12; ++row) {
        for (int col = 0; col < 8; ++col) {
            char px = shape[row][col];
            if (px == '.') continue;
            gfx_putpixel(x + col, y + row, px == 'X' ? 0 : 15);
        }
    }
}

static void restore_cursor_bg(void) {
    if (!cursor_drawn) return;
    int idx = 0;
    for (int row = 0; row < 12; ++row) {
        for (int col = 0; col < 8; ++col) {
            gfx_putpixel(cursor_prev_x + col, cursor_prev_y + row, cursor_bg[idx++]);
        }
    }
    cursor_drawn = 0;
}

static void save_cursor_bg(int x, int y) {
    int idx = 0;
    for (int row = 0; row < 12; ++row) {
        for (int col = 0; col < 8; ++col) {
            cursor_bg[idx++] = gfx_getpixel(x + col, y + row);
        }
    }
    cursor_prev_x = x;
    cursor_prev_y = y;
    cursor_drawn = 1;
}

static void draw_win95_button(int x, int y, int w, int h, int pressed, const char* label) {
    for (int yy = 0; yy < h; ++yy) {
        for (int xx = 0; xx < w; ++xx) {
            u8 base = ((xx ^ yy) & 1) ? 7 : 8;
            gfx_putpixel(x + xx, y + yy, base);
        }
    }

    u8 hi = pressed ? 8 : 15;
    u8 lo = pressed ? 15 : 0;

    gfx_rect(x, y, w, 1, hi);
    gfx_rect(x, y, 1, h, hi);
    gfx_rect(x, y + h - 1, w, 1, lo);
    gfx_rect(x + w - 1, y, 1, h, lo);

    int tx = x + 10 + (pressed ? 1 : 0);
    int ty = y + 7 + (pressed ? 1 : 0);
    gfx_text(tx, ty, label, 0);
}

void gui_draw(void) {
    restore_cursor_bg();

    gfx_clear(3);

    for (int y = 0; y < SCREEN_H - taskbar_h; y += 20) {
        for (int x = 0; x < SCREEN_W; x += 20) {
            gfx_rect(x, y, 1, 20, 2);
            gfx_rect(x, y, 20, 1, 2);
        }
    }

    gfx_rect(0, SCREEN_H - taskbar_h, SCREEN_W, taskbar_h, 8);
    gfx_rect(0, SCREEN_H - taskbar_h - 1, SCREEN_W, 1, 15);

    draw_win95_button(4, SCREEN_H - taskbar_h + 3, start_w, taskbar_h - 6,
                      (start_pressed || start_menu_open), "Start");

    gfx_text(70, SCREEN_H - taskbar_h + 10, "ZerO-3.0 desktop", 15);
    gfx_text(220, SCREEN_H - taskbar_h + 10, mouse_ready ? "ready:1" : "ready:0", 15);

    if (start_menu_open) {
        gfx_rect(4, SCREEN_H - taskbar_h - 82, 120, 80, 8);
        gfx_rect(5, SCREEN_H - taskbar_h - 81, 118, 78, 7);
        gfx_text(12, SCREEN_H - taskbar_h - 72, "Start", 0);
        gfx_text(12, SCREEN_H - taskbar_h - 58, "R - draw", 0);
        gfx_text(12, SCREEN_H - taskbar_h - 44, "C - clear", 0);
        gfx_text(12, SCREEN_H - taskbar_h - 30, "type key", 0);
    }

    save_cursor_bg(mouse_x, mouse_y);
    draw_cursor(mouse_x, mouse_y);
}

void gui_mouse_move(int dx, int dy) {
    if (dx == 0 && dy == 0) return;

    int old_x = mouse_x;
    int old_y = mouse_y;
    mouse_x += dx;
    mouse_y += dy;

    if (mouse_x < 0) mouse_x = 0;
    if (mouse_y < 0) mouse_y = 0;
    if (mouse_x > SCREEN_W - 8) mouse_x = SCREEN_W - 8;
    if (mouse_y > SCREEN_H - 12) mouse_y = SCREEN_H - 12;

    if (mouse_x == old_x && mouse_y == old_y) return;

    restore_cursor_bg();
    save_cursor_bg(mouse_x, mouse_y);
    draw_cursor(mouse_x, mouse_y);
}

void gui_mouse_click(int left_down) {
    if (left_down == mouse_left_down) return;
    mouse_left_down = left_down;

    int in_start = mouse_x >= 4 && mouse_x < 4 + start_w &&
                   mouse_y >= SCREEN_H - taskbar_h + 3 && mouse_y < SCREEN_H - 3;
    start_pressed = (left_down && in_start) ? 1 : 0;
    if (left_down && in_start) {
        start_menu_open = !start_menu_open;
    }
    gui_draw();
}

void gui_keyboard_input(char c) {
    if (!c) return;
    if (c == 'r' || c == 'R') {
        start_menu_open = 0;
        gui_draw();
    } else if (c == 'c' || c == 'C') {
        start_menu_open = 0;
        gui_draw();
    }
}

void gui_set_mouse_ready(int ready) {
    mouse_ready = ready ? 1 : 0;
    gui_draw();
}
