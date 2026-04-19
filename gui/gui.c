#include "gui.h"
#include "graphics.h"

static int mouse_x = SCREEN_W / 2;
static int mouse_y = SCREEN_H / 2;
static int start_pressed = 0;
static int start_menu_open = 0;
static int mouse_ready = 0;
static char input_line[40];
static int input_len = 0;

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

    u8 start_color = (start_pressed || start_menu_open) ? 12 : 10;
    gfx_rect(4, SCREEN_H - taskbar_h + 3, start_w, taskbar_h - 6, start_color);
    gfx_text(14, SCREEN_H - taskbar_h + 10, "Start", 15);

    gfx_text(70, SCREEN_H - taskbar_h + 10, "ZerO-3.0 desktop", 15);
    gfx_text(220, SCREEN_H - taskbar_h + 10, mouse_ready ? "ready:1" : "ready:0", 15);

    gfx_rect(10, 24, 300, 90, 1);
    gfx_rect(11, 25, 298, 88, 9);
    gfx_text(18, 34, "Keyboard data:", 15);
    gfx_rect(18, 46, 284, 14, 0);
    gfx_text(20, 50, input_line, 10);

    if (start_menu_open) {
        gfx_rect(4, SCREEN_H - taskbar_h - 82, 120, 80, 8);
        gfx_rect(5, SCREEN_H - taskbar_h - 81, 118, 78, 7);
        gfx_text(12, SCREEN_H - taskbar_h - 72, "Start", 0);
        gfx_text(12, SCREEN_H - taskbar_h - 58, "R - draw", 0);
        gfx_text(12, SCREEN_H - taskbar_h - 44, "C - clear", 0);
        gfx_text(12, SCREEN_H - taskbar_h - 30, "type key", 0);
    }

    draw_cursor(mouse_x, mouse_y);
}

void gui_mouse_move(int dx, int dy) {
    mouse_x += dx;
    mouse_y += dy;

    if (mouse_x < 0) mouse_x = 0;
    if (mouse_y < 0) mouse_y = 0;
    if (mouse_x >= SCREEN_W) mouse_x = SCREEN_W - 1;
    if (mouse_y >= SCREEN_H) mouse_y = SCREEN_H - 1;

    gui_draw();
}

void gui_mouse_click(int left_down) {
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

    if (c == '\b') {
        if (input_len > 0) {
            input_line[--input_len] = 0;
        }
    } else if (c == '\n') {
        input_len = 0;
        input_line[0] = 0;
    } else if (c == 'C' || c == 'c') {
        input_len = 0;
        input_line[0] = 0;
    } else if (c >= 32 && c <= 126 && input_len < (int)sizeof(input_line) - 1) {
        input_line[input_len++] = c;
        input_line[input_len] = 0;
    }

    if (c == 'r' || c == 'R') {
        start_menu_open = 0;
    }
    gui_draw();
}

void gui_set_mouse_ready(int ready) {
    mouse_ready = ready ? 1 : 0;
    gui_draw();
}
