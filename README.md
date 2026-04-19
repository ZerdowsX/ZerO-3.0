# ZerO-3.0

Минимальная низкоуровневая ОС без сторонних библиотек/фреймворков:
- BIOS bootloader на NASM (16-bit),
- переход в 32-bit protected mode,
- ядро на C с ASM-вставками,
- графика VGA Mode 13h,
- простой GUI (desktop + taskbar + кнопка Start),
- драйверы клавиатуры (IRQ1) и PS/2 мыши (IRQ12).

## Структура проекта

```text
bootloader/
  boot.asm          # stage1, BIOS MBR, загрузка stage2
  stage2.asm        # 16-bit setup, VGA mode, A20, switch to protected mode
kernel/
  entry.asm         # точка входа в 32-bit ядро
  interrupts.asm    # IRQ stubs + lidt helper
  idt.c             # IDT + PIC remap
  kernel.c          # main loop

drivers/
  keyboard.c        # scancode обработка, IRQ1
  mouse.c           # PS/2 mouse init + IRQ12 packets

graphics/
  graphics.c        # пиксели, прямоугольники, текст

gui/
  gui.c             # desktop, taskbar, Start, cursor

include/
  *.h               # общие заголовки

linker.ld           # линковка ядра на 0x10000
Makefile            # сборка raw image + ISO + запуск в QEMU
```

## Требования

- `nasm`
- `i686-elf-gcc`, `i686-elf-ld`, `i686-elf-objcopy`
- `qemu-system-i386`
- `xorriso` (для ISO цели)

## Сборка (по умолчанию сразу ISO)

```bash
make
```

Результат: `zero3.iso` и `os-image.bin`.

Если нужен только raw образ:

```bash
make os-image.bin
```

## Запуск

```bash
make run
```

## Сборка ISO

```bash
make iso
```

Результат: `zero3.iso`

Запуск ISO:

```bash
make run-iso
```

## Управление

- Мышь: перемещение курсора + левый клик по кнопке `Start` (визуальная реакция).
- Клавиатура: нажатие `r`/`R` перерисовывает desktop.
