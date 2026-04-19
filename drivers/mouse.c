#include "mouse.h"
#include "io.h"
#include "idt.h"
#include "gui.h"

static u8 packet[3];
static u8 packet_idx = 0;

static void mouse_wait_write(void) {
    while (inb(0x64) & 0x02) {}
}

static void mouse_wait_read(void) {
    while (!(inb(0x64) & 0x01)) {}
}

static void mouse_write(u8 data) {
    mouse_wait_write();
    outb(0x64, 0xD4);
    mouse_wait_write();
    outb(0x60, data);
}

static u8 mouse_read(void) {
    mouse_wait_read();
    return inb(0x60);
}

void mouse_init(void) {
    mouse_wait_write();
    outb(0x64, 0xA8);

    mouse_wait_write();
    outb(0x64, 0x20);
    u8 status = mouse_read() | 0x02;

    mouse_wait_write();
    outb(0x64, 0x60);
    mouse_wait_write();
    outb(0x60, status);

    mouse_write(0xF6);
    mouse_read();
    mouse_write(0xF4);
    mouse_read();
}

void irq12_handler(void) {
    u8 data = inb(0x60);
    packet[packet_idx++] = data;

    if (packet_idx == 3) {
        packet_idx = 0;
        int dx = (packet[0] & 0x10) ? (int)packet[1] - 256 : packet[1];
        int dy = (packet[0] & 0x20) ? (int)packet[2] - 256 : packet[2];
        gui_mouse_move(dx, -dy);
        gui_mouse_click(packet[0] & 0x1);
    }

    irq_ack(12);
}
