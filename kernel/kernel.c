#include "idt.h"
#include "graphics.h"
#include "gui.h"
#include "keyboard.h"
#include "mouse.h"

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
    gfx_text(8, 8, "Kernel ready", 15);

    while (1) {
        char c = keyboard_last_char();
        if (c) {
            gui_keyboard_input(c);
        }
        __asm__ volatile ("hlt");
    }
}
