[org 0x7C00]
[bits 16]

KERNEL_STAGE2_SEG equ 0x0800
KERNEL_STAGE2_OFFSET equ 0x0000
STAGE2_SECTORS equ 4

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov [boot_drive], dl

    mov si, msg_loading
    call puts

    mov ax, KERNEL_STAGE2_SEG
    mov es, ax
    xor bx, bx

    mov ah, 0x02
    mov al, STAGE2_SECTORS
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x00
    mov dl, [boot_drive]
    int 0x13
    jc disk_error

    jmp KERNEL_STAGE2_SEG:KERNEL_STAGE2_OFFSET

disk_error:
    mov si, msg_disk_error
    call puts
    hlt
    jmp $

puts:
    mov ah, 0x0E
.next:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .next
.done:
    ret

boot_drive db 0
msg_loading db 'Loading stage2...', 0
msg_disk_error db 'Disk read error', 0

times 510-($-$$) db 0
dw 0xAA55
