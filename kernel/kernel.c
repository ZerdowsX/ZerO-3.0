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
    gfx_text(8, 8, "Kernel ready", 15);

    while (1) {
        char c = keyboard_last_char();
        if (c == 'r' || c == 'R') {
            gui_draw();
            gfx_text(8, 8, "Desktop redraw", 15);
        }
        __asm__ volatile ("hlt");
    }
}
