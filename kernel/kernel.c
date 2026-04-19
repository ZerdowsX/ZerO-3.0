#include "idt.h"
#include "graphics.h"
#include "gui.h"
#include "keyboard.h"
#include "mouse.h"
#include "timer.h"

static void boot_splash(void) {
    gfx_clear(0);
    gfx_rect(0, 0, SCREEN_W, SCREEN_H, 1);
    gfx_rect(8, 8, SCREEN_W - 16, SCREEN_H - 16, 0);
    gfx_text(76, 92, "ZerOS 2.0 by Zerdows", 15);

    for (volatile int i = 0; i < 25000000; ++i) {
        __asm__ volatile ("" ::: "memory");
    }
}

void kmain(void) {
    boot_splash();

    idt_init();
    keyboard_init();
    mouse_init();

    gui_init();
    gui_set_mouse_ready(mouse_is_ready());
    gui_set_clock_seconds(0);
    gfx_text(8, 8, "Kernel ready", 15);

    u32 shown_seconds = 0;
    int kb_click = 0;
    while (1) {
        u32 now = timer_seconds();
        if (now != shown_seconds) {
            shown_seconds = now;
            gui_set_clock_seconds(now);
        }

        char c = keyboard_last_char();
        if (c) {
            gui_keyboard_input(c);
            if (!mouse_is_ready()) {
                if (c == 'w' || c == 'W') gui_mouse_move(0, -3);
                if (c == 's' || c == 'S') gui_mouse_move(0, 3);
                if (c == 'a' || c == 'A') gui_mouse_move(-3, 0);
                if (c == 'd' || c == 'D') gui_mouse_move(3, 0);
                if (c == ' ') {
                    kb_click = !kb_click;
                    gui_mouse_click(kb_click);
                }
            }
        }
        __asm__ volatile ("hlt");
    }
}
