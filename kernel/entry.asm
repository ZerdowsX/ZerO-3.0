[bits 32]
section .text
global _start
extern kmain

_start:
    cli
    mov esp, 0x9FB00
    call kmain
.halt:
    hlt
    jmp .halt
