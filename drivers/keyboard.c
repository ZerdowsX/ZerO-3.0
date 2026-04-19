#include "keyboard.h"
#include "idt.h"
#include "io.h"

static volatile char g_last = 0;

static const char scancode_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z',
    'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0
};

void keyboard_init(void) {}

char keyboard_last_char(void) {
    char c = g_last;
    g_last = 0;
    return c;
}

void irq1_handler(void) {
    u8 sc = inb(0x60);
    if (!(sc & 0x80) && sc < sizeof(scancode_map)) {
        g_last = scancode_map[sc];
    }
    irq_ack(1);
}
