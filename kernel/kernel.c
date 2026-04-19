#include "idt.h"
#include "graphics.h"
#include "gui.h"
#include "keyboard.h"
#include "mouse.h"

void kmain(void) {
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
            if (c == 'r' || c == 'R') {
                gfx_text(8, 8, "Desktop redraw", 15);
            }
        }
        __asm__ volatile ("hlt");
    }
}
