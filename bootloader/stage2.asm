[org 0x8000]
[bits 16]

KERNEL_SEG equ 0x1000
KERNEL_OFF equ 0x0000
KERNEL_SECTORS equ 64
KERNEL_START_SECTOR equ 6

start_stage2:
    cli
    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0x9000

    mov [boot_drive], dl

    ; VGA mode 13h (320x200x256)
    mov ax, 0x0013
    int 0x10

    call enable_a20

    mov si, msg_kernel
    call puts

    mov ax, KERNEL_SEG
    mov es, ax
    xor bx, bx

    mov ah, 0x02
    mov al, KERNEL_SECTORS
    mov ch, 0x00
    mov cl, KERNEL_START_SECTOR
    mov dh, 0x00
    mov dl, [boot_drive]
    int 0x13
    jc disk_error

    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp CODE_SEL:protected_mode_entry

[bits 32]
protected_mode_entry:
    mov ax, DATA_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x9FC00

    jmp CODE_SEL:0x00010000

[bits 16]
enable_a20:
    in al, 0x92
    or al, 00000010b
    out 0x92, al
    ret

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

disk_error:
    mov si, msg_disk
    call puts
    hlt
    jmp $

boot_drive db 0
msg_kernel db 'Loading kernel...',0
msg_disk db 'Kernel read failed',0

align 8
gdt_start:
    dq 0
    dq 0x00CF9A000000FFFF
    dq 0x00CF92000000FFFF
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEL equ 0x08
DATA_SEL equ 0x10

; Pad to 4 sectors (2048 bytes)
times 2048-($-$$) db 0
