    .global _start
    .global call_crt0

    .section .iram0.text, "ax"
    .align 4

/*
 * Entry point called after ESP32-S3 ROM bootloader loads our app image.
 * ROM bootloader jumps to the image's entry address (set in linker script).
 */
_start:
    /* Disable interrupts during initialization */
    rsil a2, 15

    /* Set up the stack pointer.
       The linker script will define _stack_top. */
    l32r a1, _stack_top      /* a1 = SP */
    mov  sp, a1

    /* Jump to C runtime initialization (crt0.c) */
    call call_crt0

    /* Should never return */
1:
    j 1b

